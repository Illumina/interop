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
 *  @date 10/28/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include "interop/io/format/stream_util.h"
#include "interop/util/length_of.h"

namespace illumina { namespace interop { namespace io
{

    /** Place holder that does not perform a copy
     */
    template<typename Source, typename Destination>
    void copy_from(std::ostream &, const Source &, const Destination &)
    { }

    /** Copy from source to destination
     *
     * @param dst destination
     * @param src source
     */
    template<typename Source, typename Destination>
    void copy_from(std::istream &, Destination &dst, const Source &src)
    {
        dst = src;
    }
    /** Copy from source to destination
     *
     * @param dst destination
     * @param src source
     */
    template<typename Source, typename Destination>
    void copy_from(const char*, Destination &dst, const Source &src) // TODO: Simplify this entire file with templates
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
    std::streamsize stream_map(std::istream &in, ValueType &val)
    {
        ReadType read_val;
        read_binary(in, read_val);
        val = static_cast<ValueType>(read_val);
        return in.gcount();
    }
    /** Read a value of type ReadType from the given input stream
     *
     * @param in input stream
     * @param val destination value
     * @return number of characters read
     */
    template<typename ReadType, typename ValueType>
    std::streamsize stream_map(char*& in, ValueType &val)
    {
        ReadType read_val;
        read_binary(in, read_val);
        val = static_cast<ValueType>(read_val);
        return sizeof(ReadType);
    }

    /** Read a string from the given input stream
     *
     * @param in input stream
     * @param val destination string
     * @return number of characters read
     */
    inline std::streamsize stream_map(std::istream &in, std::string &val)
    {
        read_binary(in, val, "");
        return in.gcount();
    }
    /** Read a string from the given input stream
     *
     * @param in input stream
     * @param val destination string
     * @return number of characters read
     */
    inline std::streamsize stream_map(char*& in, std::string &val)
    {
        read_binary(in, val);
        return val.size();
    }

    /** Helper to read an array
     */
    template<typename ReadType, typename ValueType, bool SameSize=sizeof(ReadType)==sizeof(ValueType)>
    struct read_array_helper
    {
        /** Read an array of values of type ReadType from the given input stream
         *
         * TODO: create more efficient buffered version
         *
         * @param in input stream
         * @param vals destination array of values
         * @param n number of values to read
         * @param offset offset into the destination array
         * @return number of bytes read from the stream
         */
        static std::streamsize read_array_from_stream(std::istream &in, ValueType* vals, const size_t n, const size_t offset=0)
        {
            std::streamsize tot = 0;
            for (size_t i = 0; i < n; i++)
            {
                ReadType read_val;
                read_binary(in, read_val);
                vals[offset + i] = read_val;
                tot += in.gcount();
            }
            return tot;
        }
        /** Read an array of values of type ReadType from the given input stream
         *
         * TODO: create more efficient buffered version
         *
         * @param in input stream
         * @param vals destination array of values
         * @param n number of values to read
         * @param offset offset into the destination array
         * @return number of bytes read from the stream
         */
        static std::streamsize read_array_from_stream(char* &in, ValueType* vals, const size_t n, const size_t offset=0)
        {
            for (size_t i = 0; i < n; i++)
            {
                ReadType read_val;
                read_binary(in, read_val);
                vals[offset + i] = read_val;
            }
            return n*sizeof(ReadType);
        }
    };
    /** Helper to read an array
     *
     * Specialization for when ReadType and ValueType are the same size
     */
    template<typename ReadType, typename ValueType>
    struct read_array_helper<ReadType, ValueType, true>
    {
        /** Read an array of values of type ReadType from the given input stream
         *
         * TODO: create more efficient buffered version
         *
         * @param in input stream
         * @param vals destination array of values
         * @param n number of values to read
         * @param offset offset into the destination array
         * @return number of bytes read from the stream
         */
        static std::streamsize read_array_from_stream(std::istream &in, ValueType* vals, const size_t n, const size_t offset=0)
        {
            read_binary(in, vals+offset, n);
            return in.gcount();
        }
        /** Read an array of values of type ReadType from the given input stream
         *
         * TODO: create more efficient buffered version
         *
         * @param in input stream
         * @param vals destination array of values
         * @param n number of values to read
         * @param offset offset into the destination array
         * @return number of bytes read from the stream
         */
        static std::streamsize read_array_from_stream(char* &in, ValueType* vals, const size_t n, const size_t offset=0)
        {
            read_binary(in, vals+offset, n);
            return n*sizeof(ReadType);
        }
    };

    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param n number of values to read
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType>
    std::streamsize stream_map(std::istream &in, std::vector<ValueType>&vals, const size_t n)
    {
        vals.resize(n);
        INTEROP_ASSERTMSG(!vals.empty(), "n="<<n);
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, &vals.front(), n);
    }


    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param n number of values to read
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType>
    std::streamsize stream_map(char*& in, std::vector<ValueType>&vals, const size_t n)
    {
        vals.resize(n);
        INTEROP_ASSERT(!vals.empty());
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, &vals.front(), n);
    }

    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param offset starting index of values to copy to in vals
     * @param n number of values to read
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType>
    std::streamsize stream_map(std::istream &in, std::vector<ValueType> &vals, const size_t offset, const size_t n)
    {
        vals.resize(offset + n);
        INTEROP_ASSERT(!vals.empty());
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, &vals.front(), n, offset);
    }

    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param offset starting index of values to copy to in vals
     * @param n number of values to read
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType>
    std::streamsize stream_map(char*& in, std::vector<ValueType> &vals, const size_t offset, const size_t n)
    {
        vals.resize(offset + n);
        INTEROP_ASSERT(!vals.empty());
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, &vals.front(), n, offset);
    }

    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param n number of values in array
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType, size_t N>
    std::streamsize stream_map(std::istream &in, ValueType (&vals)[N], const size_t n)
    {
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, vals, n);
    }

    /** Read an array of values of type ReadType from the given input stream
     *
     * TODO: create more efficient buffered version
     *
     * @param in input stream
     * @param vals destination array of values
     * @param n number of values in array
     * @return number of bytes read from the stream
     */
    template<typename ReadType, typename ValueType, size_t N>
    std::streamsize stream_map(char* &in, ValueType (&vals)[N], const size_t n)
    {
        return read_array_helper<ReadType,ValueType>::read_array_from_stream(in, vals, n);
    }

    /** Write a value of type ReadType to the given output stream
     *
     * @param out output stream
     * @param val source value
     * @return number of bytes written to the stream
     */
    template<typename WriteType, typename ValueType>
    std::streamsize stream_map(std::ostream &out, const ValueType val)
    {
        write_binary(out, static_cast<WriteType>(val));
        return out.tellp();
    }

    /** Write a string to the given output stream
     *
     * TODO: create more efficient buffered version
     *
     * @param out output stream
     * @param str source string
     * @return number of bytes written to the stream
     */
    inline std::streamsize stream_map(std::ostream &out, const std::string &str)
    {
        write_binary(out, str);
        return out.tellp();
    }

    /** Write an array of values of type ReadType to the given output stream
     *
     * TODO: create more efficient buffered version
     *
     * @param out output stream
     * @param vals destination array of values
     * @param n number of values in array
     * @return number of bytes written to the stream
     */
    template<typename WriteType, typename ValueType>
    std::streamsize stream_map(std::ostream &out, const ValueType &vals, const size_t n)
    {
        INTEROP_ASSERT(util::length_of(vals) >= n);
        if(util::length_of(vals) < n)
            INTEROP_THROW(bad_format_exception, "Write bug: Number of values is less than expected!");
        for (size_t i = 0; i < n; i++)
        {
            WriteType write_val = static_cast<WriteType>(vals[i]);
            write_binary(out, write_val);
        }
        return out.tellp();
    }

    /** Write an array of values of type ReadType to the given output stream
     *
     * TODO: create more efficient buffered version
     *
     * @param out output stream
     * @param vals destination array of values
     * @param offset starting index of values to write to in vals
     * @param n number of values in array
     * @return number of bytes written to the stream
     */
    template<typename WriteType, typename ValueType>
    std::streamsize stream_map(std::ostream &out, const ValueType &vals, const size_t offset, const size_t n)
    {
        INTEROP_ASSERT(util::length_of(vals) >= (offset+n));
        if(util::length_of(vals) < (offset+n))
            INTEROP_THROW(bad_format_exception, "Write bug: Number of values is less than expected!");
        for (size_t i = 0; i < n; i++)
        {
            WriteType write_val = static_cast<WriteType>(vals[offset + i]);
            write_binary(out, write_val);
        }
        return out.tellp();
    }

    /** Placeholder that does nothing
     */
    template<typename Layout>
    void map_resize(const Layout &, size_t)
    {
    }

    /** Resize both vectors
     *
     * @param layout vector of layout values
     * @param n number of elements
     */
    template<typename Layout>
    void map_resize(Layout &layout, const size_t n)
    {
        layout.resize(n);
    }

}}}


