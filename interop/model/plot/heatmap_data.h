/** Encapsulates all the data for a heat map
 *
 *  @file
 *  @date 5/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/assert.h"
#include "interop/util/exception.h"
#include "interop/model/plot/series.h"
#include "interop/model/plot/axes.h"
#include "interop/model/plot/chart_data.h"
#include "interop/io/table/csv_format.h"

namespace illumina { namespace interop { namespace model { namespace plot
{

    /** Encapsulates all data for a heatmap
     */
    class heatmap_data : public chart_data
    {

    public:
        /** Constructor */
        heatmap_data() : m_data(0), m_num_columns(0), m_num_rows(0), m_free(false)
        { }
        /** Destructor */
        virtual ~heatmap_data()
        {
            clear();
        }

    public:
        /** Resize the heat map to the given number of rows and columns
         *
         * @param data use the given buffer to back the heat map
         */
        void set_buffer(float* data) throw(invalid_parameter)
        {
            if(m_free) INTEROP_THROW( invalid_parameter, "Cannot use internal buffer map with external buffer");
            if(empty()) INTEROP_THROW( invalid_parameter, "Cannot set external buffer to empty map");
            m_data = data;
        }
        /** Resize the heat map to the given number of rows and columns
         *
         * @param data use the given buffer to back the heat map
         * @param rows number of rows
         * @param cols number of columns
         * @param default_val value to fill heatmap
         */
        void set_buffer(float* data,
                        const size_t rows,
                        const size_t cols,
                        const float default_val=std::numeric_limits<float>::quiet_NaN())
        {
            if(m_free) delete[] m_data;
            m_data = data;
            m_num_columns = cols;
            m_num_rows = rows;
            m_free=false;
            std::fill(data, data+length(), default_val);
        }
        /** Resize the heat map to the given number of rows and columns
         *
         * @param rows number of rows
         * @param cols number of columns
         * @param default_val value to fill heatmap
         */
        void resize(const size_t rows, const size_t cols,
                    const float default_val=std::numeric_limits<float>::quiet_NaN())
        {
            if(rows != m_num_rows && cols != m_num_columns)
            {
                if (m_free) delete[] m_data;
                m_data = new float[cols * rows];
                m_num_columns = cols;
                m_num_rows = rows;
                m_free = true;
                std::fill(m_data, m_data+length(), default_val);
            }
        }

        /** Get value at given row and column
         *
         * TODO: This should thrown an exception if wrong
         *
         * @deprecated Will be removed in next feature version (use operator() instead for C++ Code)
         * @param row row index
         * @param col column index
         * @return value
         */
        float at(const size_t row, const size_t col) const throw(model::index_out_of_bounds_exception)
        {
            if (row >= m_num_rows)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            if (col >= m_num_columns)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Column index out of bounds (at): " << col << " >= " << m_num_columns);
            const size_t idx = index_of(row, col);
            INTEROP_ASSERT(idx < m_num_rows*m_num_columns);
            INTEROP_ASSERT(m_data != 0);
            return m_data[idx];
        }

        /** Get value at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param idx index
         * @return value
         */
        float at(const size_t idx) const throw(model::index_out_of_bounds_exception)
        {
            if (idx >= length())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Index out of bounds");
            INTEROP_ASSERT(idx < m_num_rows*m_num_columns);
            INTEROP_ASSERT(m_data != 0);
            return m_data[idx];
        }
        /** Get value at given index
         *
         * @param idx index
         * @return value
         */
        float operator[](const size_t idx) const throw(model::index_out_of_bounds_exception)
        {
            if (idx >= length())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Index out of bounds");
            INTEROP_ASSERT(idx < m_num_rows*m_num_columns);
            INTEROP_ASSERT(m_data != 0);
            return m_data[idx];
        }

        /** Get value at given row and column
         *
         * TODO: This should thrown an exception if wrong
         *
         * @param row row index
         * @param col column index
         * @return value
         */
        float operator()(const size_t row, const size_t col) const throw(model::index_out_of_bounds_exception)
        {
            if (row >= m_num_rows)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            if (col >= m_num_columns)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Column index out of bounds (operator const): " << col << " >= " << m_num_columns);
            const size_t idx = index_of(row, col);
            INTEROP_ASSERT(idx < m_num_rows*m_num_columns);
            INTEROP_ASSERT(m_data != 0);
            return m_data[idx];
        }

        /** Get value at given row and column
         *
         * TODO: This should thrown an exception if wrong
         *
         * @param row row index
         * @param col column index
         * @return value
         */
        float &operator()(const size_t row, const size_t col) throw(model::index_out_of_bounds_exception)
        {
            if (row >= m_num_rows)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            if (col >= m_num_columns)
                INTEROP_THROW(model::index_out_of_bounds_exception, "Column index out of bounds (operator): " << col << " >= " << m_num_columns);
            const size_t idx = index_of(row, col);
            INTEROP_ASSERT(idx < m_num_rows*m_num_columns);
            INTEROP_ASSERT(m_data != 0);
            return m_data[idx];
        }

        /** Get number of rows
         *
         * @return number of rows
         */
        size_t row_count() const
        {
            return m_num_rows;
        }

        /** Get number of columns
         *
         * @return number of columns
         */
        size_t column_count() const
        {
            return m_num_columns;
        }

        /** Get number of rows * columns
         *
         * @return number of rows * columns
         */
        size_t length() const
        {
            return m_num_columns * m_num_rows;
        }
        /** Test if heatmap is empty
         *
         * @return true if there are not values in heatmap
         */
        bool empty()const
        {
            return length()==0;
        }

        /** Clear the data
         */
        void clear()
        {
            if(m_free)
            {
                delete[] m_data;
                m_data=0;
                m_free = false;
            }
            m_num_columns = 0;
            m_num_rows = 0;
            chart_data::clear();
        }
        /** Get the index of the row and column in the array
         *
         * @param row row index
         * @param col column index
         * @return index in array
         */
        size_t index_of(const size_t row, const size_t col) const
        {
            return row * m_num_columns + col;
        }
        friend std::ostream& operator<<(std::ostream& out, const heatmap_data& data)
        {
            out << static_cast<const chart_data&>(data);
            out << data.m_num_columns << ",";
            out << data.m_num_rows << ",";
            io::table::write_csv(out, data.m_data, data.m_data+data.length(), ',');
            return out;
        }
        friend std::istream& operator>>(std::istream& in, heatmap_data& data)
        {
            in >> static_cast<chart_data&>(data);
            std::string tmp;
            size_t col_count, row_count;
            io::table::read_value<size_t>(in, col_count, tmp);
            io::table::read_value<size_t>(in, row_count, tmp);
            data.resize(row_count, col_count);
            io::table::read_csv(in, data.m_data, data.m_data+data.length());
            return in;
        }

    private:
        float* m_data;
        size_t m_num_columns;
        size_t m_num_rows;
        bool m_free;
    };

}}}}

