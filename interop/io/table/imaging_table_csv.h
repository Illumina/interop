/** Write an imaging table into a CSV file
 *
 *  @file
 *  @date 7/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/io/table/csv_format.h"
#include "interop/model/table/imaging_table.h"
#include "interop/logic/table/create_imaging_table_columns.h"
#include "interop/logic/table/create_imaging_table.h"

namespace illumina { namespace interop { namespace model { namespace table
{


    /** Read a column header from an input stream
     *
     * @param in input stream
     * @param column column header
     * @return input stream
     */
    inline std::istream& operator>>(std::istream& in, imaging_column& column)
    {
        std::getline(in, column.m_name, ',');
        std::string::size_type n;
        if((n=column.m_name.find("<")) != std::string::npos)
        {
            column.m_subcolumn_names.clear();
            std::istringstream iss(column.m_name.substr(n+1, column.m_name.length()-1-n-1));
            std::string tmp;
            while(iss.good())
            {
                std::getline(iss, tmp, ';');
                if(!tmp.empty()) column.m_subcolumn_names.push_back(tmp);
                else break;
            }
            column.m_name = column.m_name.substr(0, n);
        }
        return in;
    }

    /** Write a column header to an output stream
     *
     * @param out output stream
     * @param column column header
     * @return output stream
     */
    inline std::ostream& operator<<(std::ostream& out, const imaging_column& column)
    {
        if(column.has_children())
        {
            out << column.name() << "<" << column.subcolumns()[0];
            for(size_t i=1;i<column.subcolumns().size();++i)
                out << ";" << column.subcolumns()[i];
            out << ">";
        }
        else out << column.name();
        return out;
    }

    /** Read an imaging table from an input stream in the CSV format
     *
     * @param in input stream
     * @param table imaging table
     * @return input stream
     */
    inline std::istream &operator>>(std::istream &in, imaging_table &table)
    {
        imaging_table::column_vector_t cols;
        io::table::read_csv_line(in, cols);
        if (!in.good()) return in;
        logic::table::populate_column_offsets(cols);

        const size_t column_count = logic::table::count_table_columns(cols);
        size_t row_count = 0;
        imaging_table::data_vector_t data;
        imaging_table::data_vector_t values;
        values.reserve(column_count);
        while (!in.eof())
        {
            io::table::read_csv_line(in, values, std::numeric_limits<float>::quiet_NaN());
            if (values.empty()) continue;
            if(column_count != values.size())
                INTEROP_THROW(io::bad_format_exception, "Number of values does not match number of columns");
            data.resize(data.size()+column_count);
            std::copy(values.begin(), values.end(), data.begin()+data.size()-column_count);
            ++row_count;
        }
        table.set_data(row_count, cols, data);
        return in;
    }
    /** Write the imaging table to the output stream in the CSV format
     *
     * @param out output stream
     * @param table imaging table
     * @return output stream
     */
    inline std::ostream &operator<<(std::ostream &out, const imaging_table &table)
    {
        if (!out.good()) return out;
        io::table::write_csv_line(out, table.m_columns);
        if (!out.good()) return out;
        for (size_t row=0,offset=0;row<table.m_row_count;++row)
        {
            io::table::write_csv_line(out, table.m_data, offset, offset+table.m_col_count);
            offset += table.m_col_count;
            if (!out.good())return out;
        }
        return out;
    }
}}}}

