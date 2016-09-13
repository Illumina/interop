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
        generator_ptr(const generator_ptr<T>& other) : m_ptr(other.m_ptr->clone()){}
        /** Copy operator
         *
         * @param other source object to copy
         * @return this
         */
        generator_ptr& operator=(const generator_ptr<T>& other)
        {
            delete m_ptr;
            m_ptr= other.m_ptr->clone();
            return *this;
        }
        /** Destructor */
        virtual ~generator_ptr(){delete m_ptr;}
        /** Generate the expected and actual metric sets
         *
         * @param expected expected object
         * @param actual actual object
         */
        bool generate(T& expected, T& actual)const
        {
            INTEROP_ASSERT(m_ptr != 0);
            return m_ptr->generate(expected, actual);
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
        //typedef ::testing::TestWithParam<  abstract_generator<T>* > parent_type;
        typedef ::testing::TestWithParam<  generator_ptr<T> > parent_type;
    public:
        //typedef  abstract_generator<T>* generator_type;
        typedef  generator_ptr<T> generator_type;
        /** Value type of the object to test */
        typedef T value_type;

        /** Constructor */
        generic_test_fixture()//const char* test_dir)
        {
            test = parent_type::GetParam().generate(expected, actual);
        }
        /** Expected object to test */
        T expected;
        /** Actual object to test */
        T actual;
        /** Run test */
        bool test;
    };


}}}
