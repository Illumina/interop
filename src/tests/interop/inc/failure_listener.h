/** Configure Google Test to only print failures
 *
 *  @file
 *  @date 6/23/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>

namespace illumina{ namespace interop { namespace unittest
{

    /** Event listener that only handles test failures */
    class failure_listener : public ::testing::TestEventListener
    {
    protected:
        /** Pointer to default event listener */
        ::testing::TestEventListener *m_event_listener;

    public:
        /** Reuse GTest's default listener
         *
         * @param event_listener gtests dfault listener
         */
        explicit failure_listener(::testing::TestEventListener *event_listener) : m_event_listener(event_listener)
        {
        }

    public:
        /** Destructor */
        virtual ~failure_listener()
        {
            delete m_event_listener;
        }
        /** Print message on program start
         *
         * @param unit_test unit test
         */
        virtual void OnTestProgramStart(const ::testing::UnitTest &unit_test)
        {
            m_event_listener->OnTestProgramStart(unit_test);
        }
        /** Print message on iteration start
         *
         * @param unit_test unit test
         * @param iteration current iteration
         */
        virtual void OnTestIterationStart(const ::testing::UnitTest &unit_test, int iteration)
        {
            m_event_listener->OnTestIterationStart(unit_test, iteration);
        }
        /** Does nothing */
        virtual void OnEnvironmentsSetUpStart(const ::testing::UnitTest &/*unit_test*/)
        {
        }

        /** Does nothing */
        virtual void OnEnvironmentsSetUpEnd(const ::testing::UnitTest &/*unit_test*/)
        {
        }

        /** Does nothing */
        virtual void OnTestCaseStart(const ::testing::TestCase &/*test_case*/)
        {
        }

        /** Does nothing */
        virtual void OnTestStart(const ::testing::TestInfo &/*test_info*/)
        {
        }

        /** Print on partial result
         *
         * @param result testing result
         */
        virtual void OnTestPartResult(const ::testing::TestPartResult &result)
        {
            m_event_listener->OnTestPartResult(result);
        }
        /** Print on test fail
         *
         * @param test_info test information
         */
        virtual void OnTestEnd(const ::testing::TestInfo &test_info)
        {
            if (test_info.result()->Failed())
            {
                m_event_listener->OnTestEnd(test_info);
            }
        }

        /** Does nothing */
        virtual void OnTestCaseEnd(const ::testing::TestCase &/*test_case*/)
        {
        }

        /** Does nothing */
        virtual void OnEnvironmentsTearDownStart(const ::testing::UnitTest &/*unit_test*/)
        {
        }
        /** Does nothing */
        virtual void OnEnvironmentsTearDownEnd(const ::testing::UnitTest &/*unit_test*/)
        {

        }
        /** Print message on iteration end
         *
         * @param unit_test unit test
         * @param iteration current iteration
         */
        virtual void OnTestIterationEnd(const ::testing::UnitTest &unit_test, int iteration)
        {
            m_event_listener->OnTestIterationEnd(unit_test, iteration);
        }

        /** Print message on program end
         *
         * @param unit_test unit test
         */
        virtual void OnTestProgramEnd(const ::testing::UnitTest &unit_test)
        {
            m_event_listener->OnTestProgramEnd(unit_test);
        }

    };
}}}


