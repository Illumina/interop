/** Write data in the CSV format
 *
 *  @file
 *  @date 6/22/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/lexical_cast.h"
#include "interop/util/math.h"


namespace illumina { namespace interop { namespace io {  namespace  table
{
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
    /** Read delimited value from the input stream and cast to proper destination type
     *
     * @param in input stream
     * @param buf reusable buffer
     * @param delim deliminator
     * @return destintion value
     */
    template<typename T>
    T read_value(std::istream& in, std::string& buf, const char delim=',')
    {
        std::getline(in, buf, delim);
        return util::lexical_cast<T>(buf);
    }
    /** Read delimited value from the input stream and cast to proper destination type
     *
     * @param in input stream
     * @param dest destination type
     * @param buf reusable buffer
     * @param delim deliminator
     */
    template<typename T>
    void read_value(std::istream& in, T& dest, std::string& buf, const char delim=',')
    {
        std::getline(in, buf, delim);
        dest = util::lexical_cast<T>(buf);
    }
    /** Read a csv values into a preallocated buffer
     *
     * @param in input stream
     * @param beg start of the buffer
     * @param end end of the buffer
     * @param delim deliminator
     */
    template<typename I>
    void read_csv(std::istream& in, I beg, I end, const char delim=',')
    {
        std::string buf;
        for(;beg != end;++beg)
        {
            read_value(in, *beg, buf, delim);
        }
    }
    /** Ignore non-float values
     *
     * @todo: Make this work for any floating point type
     * @param val non-float value
     * @return value
     */
    template<class T>
    inline const T& handle_nan(const T& val)
    {
        return val;
    }
    /** Ensure we get consistent NaNs
     *
     * @param val float value
     * @return float value
     */
    inline float handle_nan(const float val)
    {
        if(std::isnan(val)) return std::numeric_limits<float>::quiet_NaN();
        return val;
    }
    /** Ensure we get consistent NaNs
     *
     * @param val float value
     * @return float value
     */
    inline double handle_nan(const double val)
    {
        if(std::isnan(val)) return std::numeric_limits<double>::quiet_NaN();
        return val;
    }
    /** Write a vector of values as a single in a CSV file
     *
     * @param out output stream
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param eol end of line terminator character
     * @param precision number of digits for floating point number
     */
    template<typename I>
    void write_csv(std::ostream& out, I beg, I end, const char eol, const size_t precision=10)
    {
        if(beg == end) return;
        std::ios::fmtflags previous_state( out.flags() );
        out << handle_nan(*beg);
        ++beg;
        for(;beg != end;++beg)
        {
            if(precision>0)
                out << "," << std::setprecision(precision) << handle_nan(*beg);
            else
                out << "," << handle_nan(*beg);
        }
        if(eol != '\0') out << eol;
        out.flags(previous_state);
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
        INTEROP_ASSERT(last <= values.size());
        write_csv(out, values.begin()+beg, values.begin()+std::min(last, values.size()), '\n');
    }

}}}}

