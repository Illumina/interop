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

    /** Abstract interface for generators
     */
    template<class T>
    class abstract_generator
    {
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected object
         * @param actual actual object
         */
        virtual bool generate(T& expected, T& actual)=0;
        /** Destructor */
        virtual ~abstract_generator(){}
    };


    /** Generic test fixture
     *
     * This fixture will generated an expected and and actual object of any type. The objects are populated by
     * parameters passed into GTest parameter interface.
     */
    template<class T>
    struct generic_test_fixture : public ::testing::TestWithParam< abstract_generator<T>* >
    {
    private:
        typedef ::testing::TestWithParam<  abstract_generator<T>* > parent_type;
    public:
        typedef  abstract_generator<T>* generator_type;
        /** Value type of the object to test */
        typedef T value_type;

        /** Constructor */
        generic_test_fixture()//const char* test_dir)
        {
            INTEROP_ASSERT(0 != parent_type::GetParam());
            test = parent_type::GetParam()->generate(expected, actual);
        }
        /** Expected object to test */
        T expected;
        /** Actual object to test */
        T actual;
        /** Run test */
        bool test;
    };


}}}
