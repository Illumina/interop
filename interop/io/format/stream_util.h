/** Stream utility functions for reading/writing binary data
 *
 *
 *  @file
 *
 *  @date 9/8/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once


#include <istream>
#include <cstddef>
#include <vector>
#include "interop/io/stream_exceptions.h"
#include "interop/util/lexical_cast.h"
#include "interop/util/cstdint.h"

namespace illumina {
    namespace interop {
        namespace io {

            /** Indicates no value should be read from or written to the stream
             */
            struct no_value
            {
                /** Convert to unsigned integer
                 *
                 * This is a sentinel value used only by index metrics. The actual index metric returns
                 * a value of 1, but the full record size calculated includes the lane,tile,read, which
                 * is of size 6. So, 6+1=7.
                 * @return 7
                 */
                operator size_t()const{return 7;}
            };

            /** Read an array of data from an input stream
             *
             * @param in input stream
             * @param buffer array of data
             * @param n number of elements in the array
             */
            template<class T>
            void read_binary(std::istream &in, T *buffer, const size_t n)
            {
                in.read(reinterpret_cast<char *>(buffer), n*sizeof(T));
            }
            /** Read an value of binary data from an input stream
             *
             * @param in input stream
             * @param buffer value
             */
            template<class T>
            void read_binary(std::istream &in, T &buffer)
            {
                read_binary(in, &buffer, 1);
            }
            /** Read an value of binary data from an input stream
             *
             * @param in input stream
             * @return buffer value
             */
            template<class T>
            T read_binary(std::istream &in)
            {
                T val;
                read_binary(in, &val, 1);
                return val;
            }
            /** Read nothing from the stream
             *
             * @return 1
             */
            template<>
            inline no_value read_binary< no_value >(std::istream &)
            {
                return no_value();
            }
            /** Read a vector of data from an input stream
             *
             * @param in input stream
             * @param buffer vector of data
             * @param n number of elements in the vector
             */
            template<class T>
            void read_binary(std::istream &in, std::vector<T> &buffer, const size_t n)
            {
                read_binary(in, &buffer.front(), n);
            }
            /** Read a vector of data from an input stream
             *
             * @param in input stream
             * @param buffer vector of data
             */
            template<class T>
            void read_binary(std::istream &in, std::vector<T> &buffer)
            {
                read_binary(in, &buffer.front(), buffer.size());
            }
            /** Read string data from a binary file stream
             *
             * @param in input stream
             * @param str string buffer
             */
            inline void read_binary(std::istream& in, std::string& str)
            {
                size_t length = read_binary< ::uint16_t >(in);
                if(in.fail()) throw incomplete_file_exception("No more data after length");
                if(length > 0)
                {
                    str.assign(length, ' ');
                    read_binary(in, const_cast<char*>(str.c_str()), length);
                }
                else str = "NA";
            }

            /** Write an array of data to an output stream
             *
             * @param out output stream
             * @param buffer array of data
             * @param n number of elements in the array
             */
            template<class T>
            void write_binary(std::ostream &out, const T *buffer, const size_t n)
            {
                out.write(reinterpret_cast<const char *>(buffer), n*sizeof(T));
            }
            /** Write a value as binary data to an output stream
             *
             * @param out output stream
             * @param buffer value
             */
            template<class T>
            void write_binary(std::ostream &out, const T &buffer)
            {
                write_binary(out, &buffer, 1);
            }
            /** Write nothing to the stream
             */
            template<>
            inline void write_binary<no_value>(std::ostream &, const no_value&)
            {
            }
            /** Write an vector of data to an output stream
             *
             * @param out output stream
             * @param buffer vector of data
             * @param n number of elements in the vector
             */
            template<class T>
            void write_binary(std::ostream &out, const std::vector<T> &buffer, const size_t n)
            {
                write_binary(out, &buffer.front(), n);
            }
            /** Write an vector of data to an output stream
             *
             * @param out output stream
             * @param buffer vector of data
             */
            template<class T>
            void write_binary(std::ostream &out, const std::vector<T> &buffer)
            {
                write_binary(out, buffer, buffer.size());
            }
            /** Write string data to a binary file stream
             *
             * @param out output stream
             * @param str string buffer
             */
            inline void write_binary(std::ostream& out, const std::string& str)
            {
                ::uint16_t len = static_cast< ::uint16_t >(str.size());
                write_binary(out, len);
                if(len > 0) write_binary(out, const_cast<char*>(str.c_str()), len);
            }
            /** Number of characters extracted on the last unformatted (binary) input operation
             *
             * @param in input stream
             * @return number of characters read
             */
            inline std::streamsize scount(std::istream& in)
            {
                return in.gcount();
            }
            /** Number of characters written so far to the stream
             *
             * @param out output stream
             * @return number of characters written
             */
            inline std::streamsize scount(std::ostream& out)
            {
                return out.tellp();
            }

        }
    }
}