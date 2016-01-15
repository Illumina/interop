/** Example 2: Reporting Extraction Metrics
 *
 *
 *  @file
 *
 *  @date 11/6/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */




// @ [Reporting Extraction Metrics]
#include <iostream>
#include <iomanip>
#include <map>
#include <ctime>
#include "interop/model/metric_sets/extraction_metric_set.h"
#include "interop/io/metric_file_stream.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
int read_interop_file(const char* filename, extraction_metrics& extraction_metric_set);
int check_args(int argc);


int main(int argc, char** argv)
{
    int ret;
    if((ret = check_args(argc)) != 0) return ret;
    extraction_metrics extraction_metric_set;

    if((ret = read_interop_file(argv[1], extraction_metric_set)) != 0) return ret;

    std::time_t t = static_cast<std::time_t>(extraction_metric_set.metrics()[0].dateTime());
    //std::tm* tm = std::localtime(&t);
    std::tm* tm = std::gmtime(&t);
    if(tm != 0)
    {
        char buffer[80];
        std::strftime(buffer, 80, "%m/%d/%Y %r", tm);
        std::cout << "Time: " << buffer << std::endl;
    }
    else std::cout << "Invalid time" << std::endl;

    return 0;
}

// @ [Reporting Tile Metrics]
int check_args(int argc)
{
    if(argc != 2)
    {
        if(argc < 2) std::cerr << "No run folder" << std::endl;
        else std::cerr << "Too many arguments" << std::endl;
        return 1;
    }
    return 0;
}

int read_interop_file(const char* filename, extraction_metrics& extraction_metric_set)
{
    try {
        read_interop(filename,
                     extraction_metric_set);
    }
    catch(const incomplete_file_exception&){}// Ignore incomplete files
    catch(const bad_format_exception& ex)
    {
        std::cerr << "InterOp did not have the expected format: " << ex.what() << std::endl;
        return 1;
    }
    catch(const file_not_found_exception& ex)
    {
        std::cerr << "Count not find InterOp file: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}