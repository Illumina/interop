/** Write data in the CSV format
 *
 *  @file
 *  @date 6/22/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/lexical_cast.h"


namespace illumina { namespace interop { namespace io {  namespace  table {
    /** Read a vector of values from a single in a CSV file
     *
     * @param in input stream
     * @param values destination vector
     * @param missing sentinel for missing values
     */
    template<typename T>
    void read_csv_line(std::istream& in, std::vector<T>& values, const T missing=T())
    {
        values.clear();
        std::string line;
        std::getline(in, line);
        std::istringstream sin(line);
        std::string cell;
        while(std::getline(sin, cell, ','))
        {
            if(cell=="") values.push_back(missing);
            else values.push_back(util::lexical_cast<T>(cell));
        }
    }
    /** Write a vector of values as a single in a CSV file
     *
     * @param out output stream
     * @param values source vector of values
     * @param beg start column offset
     * @param last last column offset
     */
    template<typename T>
    void write_csv_line(std::ostream& out, const std::vector<T>& values, const size_t beg=0, size_t last=0)
    {
        if(values.empty())return;
        if(last == 0) last = values.size();
        INTEROP_ASSERT(beg < values.size());
        out << values[beg];
        INTEROP_ASSERT(last <= values.size());
        for(typename std::vector<T>::const_iterator it=values.begin()+beg+1, end=values.begin()+last;it != end;++it)
        {
            out << "," << *it;
        }
        out << "\n";
    }

}}}}
