/** Back port of unique_ptr from c++11
 *
 *
 *  @file
 *  @date 8/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once



#if defined(__cplusplus) && __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(HAVE_UNIQUE_PTR)
namespace stdbp
{
    /** Memory management for a unique pointer
     */
    template<class T>
    class unique_ptr
    {
    public:
        /** Constructor
         *
         * @param ptr pointer to object
         */
        unique_ptr(T* ptr=0) : m_ptr(ptr){}
        /** Destructor
         *
         * Delete the pointer to the object, freeing memory
         */
        ~unique_ptr(){delete m_ptr;}
        /** Copy constructor
         *
         * Move the pointer from one unique pointer to the next
         *
         * @param up unique pointer
         */
        unique_ptr(const unique_ptr<T>& up) : m_ptr(up.m_ptr){up.m_ptr = 0;}
        /** Assignment operator
         *
         * Move the pointer from one unique pointer to the next
         *
         * @param up unique pointer
         * @return this unique pointer object
         */
        unique_ptr<T>& operator=(const unique_ptr<T>& up) {m_ptr=up.m_ptr;up.m_ptr = 0;return *this;}

    public:
        /** Get pointer
         *
         * @return pointer to object
         */
        T* operator->() const {return m_ptr;}
        /** Get the object pointed to
         *
         * @return reference to object
         */
        T& operator*() const{return *m_ptr;}
        /** Test if pointer does not equal to another pointer
         *
         * @param up unique pointer
         * @return true if not equal
         */
        bool operator!=(const unique_ptr<T>& up)const
        {
            return m_ptr != up.m_ptr;
        }
        /** Test if pointer does not equal to another pointer
         *
         * @param ptr pointer held by unique pointer
         * @return true if not equal
         */
        bool operator!=(const T* ptr)const
        {
            return m_ptr != ptr;
        }
        /** Test if the unique pointer is empty
         *
         * @return true of the pointer != null
         */
        operator bool()const
        {
            return m_ptr != 0;
        }

    private:
        mutable T* m_ptr;
    };
}
#else
#include <memory>
namespace stdbp
{
    template<typename T> using unique_ptr = std::unique_ptr<T>;
}
#endif


