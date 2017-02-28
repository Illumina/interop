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
#include "interop/util/option_parser.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

template<typename I>
void zero_extraction_time(I beg, I end)
{
    for(;beg != end;++beg) beg->date_time(0);
}

/** Copy of subset of metrics
 */
struct subset_copier
{
    /** Constructor
     *
     * @param run run metrics
     * @param max_tile_number maximum tile number
     */
    subset_copier(run_metrics& run, const size_t max_tile_number) : m_run(run), m_max_tile_number(max_tile_number){}

    /** Function operator overload to collect a subset of metrics
     *
     * @param metrics set of metrics
     */
    template<class MetricSet>
    void operator()(const MetricSet& metrics)const
    {
        typedef typename MetricSet::base_t base_t;
        copy(metrics, base_t::null());
    }

private:
    template<class MetricSet>
    void copy(const MetricSet& metrics, const void*)const
    {
        m_run.get<MetricSet>() = MetricSet(metrics, metrics.version());

        for(size_t i=0;i<metrics.size();++i)
        {
            if(metrics[i].number(m_run.run_info().flowcell().naming_method()) > m_max_tile_number) continue;
            m_run.get<MetricSet>().insert(metrics[i]);
        }
    }

private:
    run_metrics& m_run;
    size_t m_max_tile_number;
};

int main(int argc, const char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    const size_t thread_count = 1;

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;

    size_t max_tile_number=0;
    util::option_parser description;
    description
            (max_tile_number, "max-tile", "Maximum tile number to include");
    if(description.is_help_requested(argc, argv))
    {
        std::cout << "Usage: " << io::basename(argv[0]) << " run_folder [--option1=value1] [--option2=value2]" << std::endl;
        description.display_help(std::cout);
        return SUCCESS;
    }
    try
    {
        description.parse(argc, argv);
        description.check_for_unknown_options(argc, argv);
    }
    catch(const util::option_exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return INVALID_ARGUMENTS;
    }


    run_metrics run;
    const std::string input_file=argv[1];
    const std::string run_name = io::basename(input_file);
    std::cout << "# Run Folder: " << run_name << std::endl;
    int ret = read_run_metrics(input_file.c_str(), run, thread_count);
    if(ret != SUCCESS) return ret;
    io::mkdir("InterOp");
    run.sort();
    zero_extraction_time(run.get<extraction_metric>().begin(), run.get<extraction_metric>().end());
    if(max_tile_number > 0)
    {
        run_metrics subset;
        subset_copier copy_subset(subset, max_tile_number);
        try
        {
            run.metrics_callback(copy_subset);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        std::cout << subset.get<model::metrics::extraction_metric>().size() << ", " << run.get<model::metrics::extraction_metric>().size() << std::endl;
        try{
            subset.write_metrics(".");
        }
        catch(const std::exception& ex)
        {
            std::cerr << "Error: Unable to write InterOp files" << std::endl;
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    else
    {
        try{
            run.write_metrics(".");
        }
        catch(const std::exception& ex)
        {
            std::cerr << "Error: Unable to write InterOp files" << std::endl;
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    return SUCCESS;
}

