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
#include "src/tests/interop/inc/regression_test_data.h"
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
    data.finalize();


    // initialize
    try
    {
        ::testing::InitGoogleTest(&argc, argv);
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }


    {
        // remove the default listener
        ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
        ::testing::TestEventListener *default_printer = listeners.Release(listeners.default_result_printer());
        // Add listener that will only print failures
        failure_listener *listener = new failure_listener(default_printer);
        listeners.Append(listener);
    }

    // run
    try
    {
        return RUN_ALL_TESTS();
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}


