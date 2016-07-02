/** Main function for unit tests
 *
 *  @file
 *  @date 6/23/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "src/tests/interop/inc/failure_listener.h"
#include "src/tests/interop/inc/regression_fixture.h"
using namespace illumina::interop::unittest;

int main(int argc, char **argv)
{
    // set baseline
    // set regression data
    regression_test_data& data = regression_test_data::instance();

    for(int i=1;i<argc;++i)
    {
        const std::string arg = argv[i];
        if(arg == "--rebaseline")
        {
            data.rebaseline(true);
            continue;
        }
        if(arg.length() >= 2 && arg.substr(0, 2) == "--") continue;
        if(data.baseline() == "")
        {
            data.baseline(arg);
            continue;
        }
        data.add_file(arg);
    }


    // initialize
    ::testing::InitGoogleTest(&argc, argv);

    // remove the default listener
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    ::testing::TestEventListener *default_printer = listeners.Release(listeners.default_result_printer());
    // Add listener that will only print failures
    failure_listener *listener = new failure_listener(default_printer);
    listeners.Append(listener);


    // run
    return RUN_ALL_TESTS();
}

