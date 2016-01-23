/** Define a generic mapping to I/O and copying values
 *
 * This generic stream_map, copy_from and map_resize method conditionally determine whether values are read
 * from or written to a stream, copied or resized.
 *
 * This allows the user to specify a single mapping in a single function, rather than
 * defining a copy from layout to metric and then another function copying metric to layout. This greatly
 * reduces the amount of code necessary to define an InterOp layout.
 *
 *  @file
 *
 *  @date 10/28/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/io/format/stream_util.h"
#include "interop/util/length_of.h"

namespace illumina {
namespace interop {
namespace io {

/** Place holder that does not perform a copy
 */
template<typename Source, typename Destination>
void copy_from(const Source&, const Destination&){}
/** Copy from the destination to the source
 *
 * @param dst destination
 * @param src source
 */
template<typename Source, typename Destination>
void copy_from(Destination& dst, const Source& src)
{
    dst = src;
}

/** Read a value of type ReadType from the given input stream
 *
 * @param in input stream
 * @param val destination value
 * @return number of characters read
 */
template<typename ReadType, typename ValueType>
std::streamsize stream_map(std::istream& in, ValueType& val)
{
    ReadType read_val;
    read_binary(in, read_val);
    val = static_cast<ValueType>(read_val);
    return in.gcount();
}

/** Read an array of values of type ReadType from the given input stream
 *
 * TODO: create more efficient buffered version
 *
 * @param in input stream
 * @param vals destinatin array of values
 * @param n number of values to read
 * @return number of bytes read from the stream
 */
template<typename ReadType, typename ValueType>
std::streamsize stream_map(std::istream& in, ValueType& vals, size_t n)
{
    std::streamsize tot = 0;
    vals.resize(n);
    for(size_t i=0;i<n;i++)
    {
        ReadType read_val;
        read_binary(in, read_val);
        vals[i] = read_val;
        tot+=in.gcount();
    }
    return tot;
}

/** Read an array of values of type ReadType from the given input stream
 *
 * TODO: create more efficient buffered version
 *
 * @param in input stream
 * @param vals destinatin array of values
 * @param n number of values in array
 * @return number of bytes read from the stream
 */
template<typename ReadType, typename ValueType, size_t N>
std::streamsize stream_map(std::istream& in, ValueType (&vals)[N], size_t n)
{
    std::streamsize tot = 0;
    for(size_t i=0;i<n;i++)
    {
        ReadType read_val;
        read_binary(in, read_val);
        vals[i] = read_val;
        tot+=in.gcount();
    }
    return tot;
}
/** Write a value of type ReadType to the given output stream
 *
 * @param out output stream
 * @param val source value
 * @return number of bytes written to the stream
 */
template<typename WriteType, typename ValueType>
std::streamsize stream_map(std::ostream& out, ValueType val)
{
    write_binary(out, static_cast<WriteType>(val));
    return out.tellp();
}

/** Write an array of values of type ReadType to the given output stream
 *
 * TODO: create more efficient buffered version
 *
 * @param out output stream
 * @param vals destinatin array of values
 * @param n number of values in array
 * @return number of bytes written to the stream
 */
template<typename WriteType, typename ValueType>
std::streamsize stream_map(std::ostream& out, const ValueType& vals, size_t n)
{
    for(size_t i=0;i<n;i++)
    {
        WriteType write_val = vals[i];
        write_binary(out, write_val);
    }
    return out.tellp();
}

/** Place older that does nothing
 */
template<typename Layout>
void map_resize(const std::vector<Layout>&, size_t)
{
}
/** Resize both vectors
 *
 * @param layout vector of layout values
 * @param n number of elements
 */
template<typename Layout>
void map_resize(std::vector<Layout>& layout, size_t n)
{
    layout.resize(n);
}

}
}
}

