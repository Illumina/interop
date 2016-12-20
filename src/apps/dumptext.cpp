/** @page dumpbin Dump Interop data into a text format
 *
 *
 * This developer application is to help debug InterOp files.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ dumptext 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-70-g9bcfb5a-dirty
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/util/option_parser.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

/** Call back functor for writing text data to the output stream
 */
struct metric_writer
{
    /** Constructor
     *
     * @param out output stream
     * @param channels list of channel names
     */
    metric_writer(std::ostream& out, const std::vector<std::string>& channels) : m_out(out), m_channel_names(channels){}
    /** Function operator overload to write data
     *
     * @param metrics set of metrics
     */
    template<class MetricSet>
    void operator()(const MetricSet& metrics)const
    {
        if(metrics.empty()) return;
        io::write_text(m_out, metrics, m_channel_names);
    }
private:
    std::ostream& m_out;
    std::vector<std::string> m_channel_names;

};
/** Copy of subset of metrics
 */
struct subset_copier
{
    /** Constructor
     *
     * @param run run metrics
     * @param total number to subsample
     */
    subset_copier(run_metrics& run, const size_t total) : m_run(run), m_total(total){}

    /** Function operator overload to collect a subset of metrics
     *
     * @param metrics set of metrics
     */
    template<class MetricSet>
    void operator()(const MetricSet& metrics)const
    {
        m_run.get<MetricSet>() = MetricSet(metrics, metrics.version());
        const size_t total = std::min(m_total, metrics.size());

        for(size_t i=0;i<total;++i)
            m_run.get<MetricSet>().insert(metrics.at(i));
    }
private:
    run_metrics& m_run;
    size_t m_total;
};

int main(int argc, const char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    const char eol = '\n';

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;

    size_t subset_count=0;
    util::option_parser description;
    description
            (subset_count, "subset", "Number of metrics to subsample");
    if(description.is_help_requested(argc, argv))
    {
        std::cout << "Usage: " << io::basename(argv[0]) << " run_folder [--option1=value1] [--option2=value2]" << std::endl;
        description.display_help(std::cout);
        return SUCCESS;
    }
    try{
        description.parse(argc, argv);
        description.check_for_unknown_options(argc, argv);
    }
    catch(const util::option_exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return INVALID_ARGUMENTS;
    }

    for(int i=1;i<argc;i++)
    {
        run_metrics run;
        int ret = read_run_metrics(argv[i], run);
        if(ret != SUCCESS) return ret;
        metric_writer write_metrics(std::cout, run.run_info().channels());
        if( subset_count > 0 )
        {
            run_metrics subset;
            subset_copier copy_subset(subset, subset_count);
            try
            {
                run.metrics_callback(copy_subset);
            }
            catch (const std::exception &ex)
            {
                std::cerr << ex.what() << std::endl;
                return UNEXPECTED_EXCEPTION;
            }
            try
            {
                subset.metrics_callback(write_metrics);
            }
            catch (const io::bad_format_exception &ex)
            {
                std::cerr << ex.what() << std::endl;
                return BAD_FORMAT;
            }
        }
        else
        {
            try
            {
                run.metrics_callback(write_metrics);
            }
            catch (const io::bad_format_exception &ex)
            {
                std::cerr << ex.what() << std::endl;
                return BAD_FORMAT;
            }
        }
        std::cout << eol << std::endl;

    }
    return SUCCESS;
}



