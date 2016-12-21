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
#include "interop/util/math.h"

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
        /** Pointer to abtract generator */
        typedef  abstract_generator<T>*  base_type;
    public:
        /** Pointer to abtract generator */
        typedef generator_ptr<T> parent_type;
        /** Constructor
         *
         * @param test_modifier flag that modifies the test
         */
        abstract_generator(const int test_modifier=0) : m_test_modifier(test_modifier){}
        /** Destructor */
        virtual ~abstract_generator(){}
        /** Generate the expected and actual metric sets
         *
         * @param expected expected object
         * @param actual actual object
         * @param skip_test flag to set if the test should be skipped
         * @return success if the results should be tested
         */
        virtual ::testing::AssertionResult generate(T& expected, T& actual, bool* skip_test)const=0;
        /** Advance to the next type
         *
         * @return true when the generator has finished, and the next parameter can be obtained
         */
        virtual bool advance()
        {
            return true;
        }

        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        virtual parent_type clone()const=0;
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

    /** Generate the plot from an empty metric set
     *
     * The expected plot is empty
     */
    template<class T, class Fixture>
    class standard_parameter_generator : public abstract_generator<T>
    {
        typedef typename Fixture::parameter_type parameter_type;
        typedef typename abstract_generator<T>::parent_type base_type;
    public:
        /** Constructor
         *
         * @param parameter parameter value
         */
        standard_parameter_generator(const parameter_type parameter) : m_fixture(parameter)
        {
        }
        /** Clone the concrete implementation TODO: Remove
         *
         * @param parameter parameter value
         * @return copy of this object
         */
        base_type operator()(const parameter_type& parameter)const
        {
            return new standard_parameter_generator(parameter);
        }

        /** Generate the expected and actual metric sets
         *
         * @param expected expected plot data
         * @param actual actual plot data
         */
        ::testing::AssertionResult generate(T& expected, T &actual, bool*) const
        {
            return m_fixture.generate(expected, actual);
        }

        /** Create a copy of this object
         *
         * @return pointer to an abstract_generator
         */
        base_type clone() const
        {
            return new standard_parameter_generator(*this);
        }

        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream &out) const
        {
            m_fixture.write(out);
        }

    protected:
        /** Fixture */
        Fixture m_fixture;
    };
    /** Generate the plot from an empty metric set
     *
     * The expected plot is empty
     */
    template<class T, class Fixture>
    class iterator_parameter_generator : public standard_parameter_generator<T, Fixture>
    {
        typedef typename Fixture::parameter_type parameter_type;
    public:
        /** Constructor
         *
         * @param parameter parameter value
         */
        iterator_parameter_generator(const parameter_type parameter) :
                standard_parameter_generator<T, Fixture>(parameter)
        {
        }
        /** Advance to the next type
         *
         * @return true when the generator has finished, and the next parameter can be obtained
         */
        bool advance()
        {
            return standard_parameter_generator<T, Fixture>::m_fixture.advance();
        }
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
        generator_ptr(const generator_ptr<T>& other) : m_ptr(0)
        {
            if(other.m_ptr != 0)
            {
                generator_ptr<T> tmp = other.m_ptr->clone();
                m_ptr = tmp.m_ptr;
                tmp.m_ptr=0;
            }
        }
        /** Copy operator
         *
         * @param other source object to copy
         * @return this
         */
        generator_ptr& operator=(const generator_ptr<T>& other)
        {
            delete m_ptr;
            INTEROP_ASSERT(other.m_ptr != 0);

            if(other.m_ptr != 0)
            {
                generator_ptr<T> tmp = other.m_ptr->clone();
                m_ptr = tmp.m_ptr;
                tmp.m_ptr=0;
            }
            return *this;
        }
        /** Copy operator
         *
         * @param ptr source object to copy
         * @return this
         */
        generator_ptr& operator=(abstract_generator<T>* ptr)
        {
            delete m_ptr;
            if(ptr != 0) m_ptr = ptr;
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
        /** Test if current pointer equals another
         *
         * @param other other pointer
         * @return true if they point to same address
         */
        bool operator==(const abstract_generator<T>* other)
        {
            return m_ptr == other;
        }
        /** Test if current pointer does not equal another
         *
         * @param other
         * @return true if they do not point to same address
         */
        bool operator!=(const abstract_generator<T>* other)
        {
            return m_ptr != other;
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
    public:
        /** Get the type of the parent */
        typedef ::testing::TestWithParam< generator_ptr<T> > parent_type;
    public:
        //typedef  abstract_generator<T>* generator_type;
        typedef  generator_ptr<T> generator_type;
        /** Value type of the object to test */
        typedef T value_type;

        /** Constructor
         */
        generic_test_fixture() : skip_test(false),
                                 fixture_test_result(parent_type::GetParam()->generate(expected, actual, &skip_test))
        {
            if(skip_test && !fixture_test_result)
                std::cout << fixture_test_result.message() << std::endl;
            test_modifier = parent_type::GetParam()->test_modifier();
        }
        /** Expected object to test */
        T expected;
        /** Actual object to test */
        T actual;
        /** Skip running the test */
        bool skip_test;
        ::testing::Message msg;
        /** Flag for type of test*/
        int test_modifier;
        /** Error trying to generate the fixture */
        const ::testing::AssertionResult fixture_test_result;
    };

    /** Test if two floats are nearly the same If both are NaN, then this check succeeds
     *
     * @param expected expected float
     * @param actual  actual float
     * @param tol tolerance
     * @return rue if both numbers hold the same value, or their difference is less than tolerance
     */
    inline bool is_float_near(const float expected, const float actual, const float tol)
    {
        if(std::isnan(expected) && std::isnan(actual)) return true;
        if(std::isnan(expected) || std::isnan(actual)) return false;
        return std::abs(expected-actual) < tol;
    }

    /** Check if two floats are nearly the same. If both are NaN, then this check succeeds
     *
     * @todo Use this everywhere
     *
     * @param expected expected float
     * @param actual actual float
     * @param tol tolerance
     * @return true if both numbers hold the same value, or their difference is less than tolerance
     */
    inline ::testing::AssertionResult AreFloatsNear(const float expected, const float actual, const float tol)
    {
        if(is_float_near(expected, actual, tol)) return ::testing::AssertionSuccess();
        return ::testing::AssertionFailure() << "Abs(" << expected << " - " << actual << ") >= " << tol;
    }

    /** Check if two float arrays are nearly the same. If both are NaN, then this check succeeds
     *
     * @todo Use this everywhere
     *
     * @param expected expected vector of floats
     * @param actual actual vector of floats
     * @param tol tolerance
     * @return true if both numbers hold the same value, or their difference is less than tolerance
     */
    inline ::testing::AssertionResult AreValuesNear(const std::vector<float>& expected,
                                                    const std::vector<float>& actual,
                                                    const float tol)
    {
        ::testing::Message msg;
        bool test_failed = false;
        if( expected.size() != actual.size() )
        {
            return ::testing::AssertionFailure() << "Expected size: " << expected.size() << " != actual: " << actual.size();
        }
        for(size_t i=0;i<expected.size();++i)
        {
            if(!is_float_near(expected[i], actual[i], tol))
            {
                if(test_failed) msg << " | ";
                msg << "Value("<< i << ") Expected: " << expected[i] << " == Actual: " << actual[i];
                test_failed=true;
            }
        }
        if(test_failed) return ::testing::AssertionFailure(msg << " Tol: " << tol);
        return ::testing::AssertionSuccess();
    }


}}}

#define INTEROP_ASSERT_NEAR(EXPECTED, ACTUAL, TOL) ASSERT_TRUE(AreFloatsNear(EXPECTED, ACTUAL, TOL))
#define INTEROP_EXPECT_NEAR(EXPECTED, ACTUAL, TOL) EXPECT_TRUE(AreFloatsNear(EXPECTED, ACTUAL, TOL))
#define INTEROP_EXPECT_ARRAY_NEAR(EXPECTED, ACTUAL, TOL) EXPECT_TRUE(AreValuesNear(EXPECTED, ACTUAL, TOL))
