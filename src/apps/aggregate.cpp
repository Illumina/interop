/** @page aggregate Aggregate by cycle InterOps
 *
 * This application aggregates InterOp files organized by cycle into a single, aggregated file for each metric.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ aggregate 140131_1287_0851_A01n401drr
 *
 * The InterOp files will be written to the current working directory into a sub folder called InterOp. All the records
 * will be sorted so two InterOp files can be compared with Linux `diff`.
 */

#include <iostream>
#include <iomanip>
#include "interop/util/filesystem.h"
#include "interop/model/run_metrics.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

template<typename I>
void zero_extraction_time(I beg, I end)
{
    for(;beg != end;++beg) beg->date_time(0);
}

int main(int argc, const char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;


    run_metrics run;
    const std::string input_file=argv[1];
    const std::string run_name = io::basename(input_file);
    std::cout << "# Run Folder: " << run_name << std::endl;
    int ret = read_run_metrics(input_file.c_str(), run);
    if(ret != SUCCESS) return ret;
    io::mkdir("InterOp");
    run.sort();
    zero_extraction_time(run.get<extraction_metric>().begin(), run.get<extraction_metric>().end());
    try{
        run.write_metrics(".");
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error: Unable to write InterOp files" << std::endl;
        std::cerr << ex.what() << std::endl;
        return UNEXPECTED_EXCEPTION;
    }
    return SUCCESS;
}
