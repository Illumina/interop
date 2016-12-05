/** Determine the length of an array or vector
 *
 * This generic function determines the length of an array or vector
 *
 *  @file
 *  @date 10/28/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>

namespace illumina { namespace interop { namespace util
{

    /** Length of a generic type
     *
     * Returns 1 by default
     */
    template<typename T>
    struct length_of_type
    {
        /** Length of generic type
         *
         * @note requires dummy parameter
         * @return 1
         */
        static size_t size(T)
        { return 1; }
    };

    /** Length of a generic vector type
     *
     * Returns std::vector::size()
     */
    template<typename T>
    struct length_of_type<std::vector<T> >
    {
        /** Length of a generic vector type
         *
         * @param vec generic vector
         * @return std::vector::size()
         */
        static size_t size(const std::vector<T> &vec)
        { return vec.size(); }
    };

    /** Get the number of elements in the collection or 1 if a value
     *
     * @param val collection or value
     * @return size of collection or 1 for value
     */
    template<typename T>
    size_t length_of(const T &val)
    { return length_of_type<T>::size(val); }

    /** Get the number of elements in a stack array
     *
     * @return size of array
     */
    template<typename T, size_t N>
    size_t length_of(const T (&)[N])
    { return N; }

    /** Convert a stack array into a std::vector
     *
     * @tparam T type held by array
     * @tparam N number of elements in array
     * @param vals stack array
     * @return std::vector
     */
    template<typename T, size_t N>
    static std::vector<T> to_vector(const T (&vals)[N])
    {
        return std::vector<T>(vals, vals + N);
    }

}}}


