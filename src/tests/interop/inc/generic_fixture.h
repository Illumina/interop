/** Framework to generate fixtures from GTest Parameters
 *
 *  @file
 *  @date 8/10/16.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/assert.h"
#include "interop/util/unique_ptr.h"

namespace illumina{ namespace interop { namespace unittest {

    /** Smart pointer wrapper
     */
    template<class T>
    class generator_ptr;
    /** Abstract interface for generators
     */
    template<class T>
    class abstract_generator
    {
    public:
        typedef  abstract_generator<T>*  base_type;
        typedef generator_ptr<T> parent_type;
        /** Constructor
         *
         * @param test_modifier flag that modifies the test
         */
        abstract_generator(const int test_modifier=0) : m_test_modifier(test_modifier){}
        /** Generate the expected and actual metric sets
         *
         * @param expected expected object
         * @param actual actual object
         */
        virtual bool generate(T& expected, T& actual)const=0;

        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        virtual base_type clone()const=0;
        /** Destructor */
        virtual ~abstract_generator(){}
        /** Flag that modifies the test
         *
         * @return flag that modifies the test
         */
        int test_modifier()const{return m_test_modifier;}
        /** Write generator info to output stream
         *
         * @param out output stream
         */
        virtual void write(std::ostream& out)const=0;
        /** Write name of generator to output stream
         *
         * @param out output stream
         * @param gen generator
         * @return output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const abstract_generator<T>& gen)
        {
            gen.write(out);
            return out;
        }

    private:
        int m_test_modifier;
    };

    /** Smart pointer wrapper
     *
     * Performs deep copies
     */
    template<class T>
    class generator_ptr
    {
    public:
        /** Constructor
         *
         * @param ptr pointer to an abstract generator
         */
        generator_ptr(abstract_generator<T>* ptr=0) : m_ptr(ptr){}
        /** Copy Constructor
         *
         * @param other source object to copy
         */
        generator_ptr(const generator_ptr<T>& other) : m_ptr(other.m_ptr==0?0:other.m_ptr->clone()){}
        /** Copy operator
         *
         * @param other source object to copy
         * @return this
         */
        generator_ptr& operator=(const generator_ptr<T>& other)
        {
            delete m_ptr;
            INTEROP_ASSERT(other.m_ptr != 0);
            if(other.m_ptr != 0) m_ptr= other.m_ptr->clone();
            return *this;
        }
        /** Destructor */
        virtual ~generator_ptr(){delete m_ptr;}

        /** Get access to underlying object
         *
         * @return reference to abstract generator
         */
        const abstract_generator<T>& operator*()const
        {
            INTEROP_ASSERT(m_ptr != 0);
            return *m_ptr;
        }
        /** Get access to underlying object
         *
         * @return reference to abstract generator
         */
        abstract_generator<T>& operator*()
        {
            INTEROP_ASSERT(m_ptr != 0);
            return *m_ptr;
        }
        /** Get access to underlying pointer
         *
         * @return pointer to abstract generator
         */
        const abstract_generator<T>* operator->()const
        {
            INTEROP_ASSERT(m_ptr != 0);
            return m_ptr;
        }
        /** Get access to underlying pointer
         *
         * @return pointer to abstract generator
         */
        abstract_generator<T>* operator->()
        {
            INTEROP_ASSERT(m_ptr != 0);
            return m_ptr;
        }
        /** Write name of generator to output stream
         *
         * @param out output stream
         * @param ptr generator pointer
         * @return output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const generator_ptr<T>& ptr)
        {
            INTEROP_ASSERT(ptr.m_ptr != 0);
            if(ptr.m_ptr == 0) return out;
            ptr.m_ptr->write(out);
            return out;
        }
    private:
        abstract_generator<T>* m_ptr;
    };
    /** Wrap the pointer in a smart pointer
     *
     * @param ptr source pointer to wrap
     * @return smart pointer
     */
    template<class T>
    generator_ptr<T> wrap(abstract_generator<T>* ptr)
    {
        return generator_ptr<T>(ptr);
    }


    /** Generic test fixture
     *
     * This fixture will generated an expected and and actual object of any type. The objects are populated by
     * parameters passed into GTest parameter interface.
     */
    template<class T>
    struct generic_test_fixture : public ::testing::TestWithParam< generator_ptr<T> >
    {
    private:
        typedef ::testing::TestWithParam<  generator_ptr<T> > parent_type;
    public:
        //typedef  abstract_generator<T>* generator_type;
        typedef  generator_ptr<T> generator_type;
        /** Value type of the object to test */
        typedef T value_type;

        /** Constructor
         */
        generic_test_fixture()
        {
            test = parent_type::GetParam()->generate(expected, actual);
            test_modifier = parent_type::GetParam()->test_modifier();
        }
        /** Expected object to test */
        T expected;
        /** Actual object to test */
        T actual;
        /** Run test */
        bool test;
        /** Flag for type of test*/
        int test_modifier;
    };


}}}

