/** @page dumptext Dump Interop data into a text format
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
 *      # Version: v1.0.14
 *      # CorrectedInt,1
 *      # Column Count: 26
 *      Lane,Tile,Cycle,AverageCycleIntensity,SignalToNoise,CalledCount_NC,CalledCount_A,CalledCount_C,CalledCount_G,CalledCount_T,CalledIntensity_A,CalledIntensity_C,CalledIntensity_G,CalledIntensity_T,AllIntensity_A,AllIntensity_C,AllIntensity_G,AllIntensity_T
 *      1,1110,25,1101,12.0613,0,574630,422083,412422,595042,4277,4260,4297,4234,1265,932,920,1286
 *      1,1110,1,1329,15.1487,7085,613597,397592,392897,593006,5220,5200,5228,5221,1639,1069,1037,1563
 *      1,1110,2,1277,14.7306,0,635012,354595,413136,601434,5036,5001,5003,4982,1626,917,1047,1514
 *      # Error,1
 *      # Column Count: 4
 *      Lane,Tile,Cycle,ErrorRate
 *      1,1110,1,0.162742
 *      1,1110,2,0.11915
 *      1,1110,3,0.0860728
 *      # Extraction,1
 *      # Column Count: 12
 *      # Channel Count: 4
 *      Lane,Tile,Cycle,TimeStamp,MaxIntensity_a,MaxIntensity_c,MaxIntensity_g,MaxIntensity_t,Focus_a,Focus_c,Focus_g,Focus_t
 *      1,1110,1,1420585636,6830,8444,3300,7030,3.09255,3.22699,2.928,2.9999
 *      1,1110,2,1420585637,6646,8197,3161,6780,3.07496,3.2106,2.9338,2.9947
 *      1,1110,3,1420585637,6581,8121,3130,6745,3.06585,3.21393,2.93435,2.99376
 *      # Q,1
 *      # Bin Count: 7
 *      # Column Count: 3
 *      Lower,Value,Upper
 *      0,6,9
 *      10,15,19
 *      20,22,24
 *      25,27,29
 *      30,33,34
 *      35,37,39
 *      40,40,49
 *      # Column Count: 10
 *      Lane,Tile,Cycle,Bin_1,Bin_2,Bin_3,Bin_4,Bin_5,Bin_6,Bin_7
 *      1,1110,1,0,11900,3569,36075,1952633,0,0
 *      1,1110,2,0,12063,4044,38008,1950062,0,0
 *      1,1110,3,0,5827,3883,42582,1951885,0,0
 *      # QByLane,1
 *      # Bin Count: 7
 *      # Column Count: 3
 *      Lower,Value,Upper
 *      0,6,9
 *      10,15,19
 *      20,22,24
 *      25,27,29
 *      30,33,34
 *      35,37,39
 *      40,40,49
 *      # Column Count: 10
 *      Lane,Tile,Cycle,Bin_1,Bin_2,Bin_3,Bin_4,Bin_5,Bin_6,Bin_7
 *      1,0,1,0,11900,3569,36075,1952633,0,0
 *      1,0,2,0,12063,4044,38008,1950062,0,0
 *      1,0,3,0,5827,3883,42582,1951885,0,0
 *      # Q2030,1
 *      # Column Count: 7
 *      Lane,Tile,Cycle,Q20,Q30,Total,MedianQScore
 *      1,1110,1,1992277,1952633,2004177,33
 *      1,1110,2,1992114,1950062,2004177,33
 *      1,1110,3,1998350,1951885,2004177,33
 *      # Tile,1
 *      # Column Count: 10
 *      Lane,Tile,Read,ClusterCount,ClusterCountPF,Density,DensityPF,Aligned,Prephasing,Phasing
 *      1,1110,1,2.08311e+06,2.00418e+06,723230,695825,1.5076,0.341797,0.128489
 *
 * ### Explanation of output
 *
 * The first line is the version of the library/executable. After that each metric starts with the metric name followed
 * by the version of the text format, e.g.
 *
 *      # Tile,1
 * Next, the metric may print header any available information such as channel count for extraction metrics, e.g.
 *
 *      # Channel Count: 4
 *
 * Every metric includes the number of columns, e.g.
 *
 *      # Column Count: 12
 *
 * There may be an extended header such as the one for Q-metrics, e.g.
 *
 *      Lower,Value,Upper
 *      0,6,9
 *      10,15,19
 *      20,22,24
 *      25,27,29
 *      30,33,34
 *      35,37,39
 *      40,40,49
 *
 * Finally, the records from the record follow a text header, e.g.
 *
 *      Lane,Tile,Cycle,Q20,Q30,Total,MedianQScore
 *      1,1110,1,1992277,1952633,2004177,33
 *
 * ### Command line options
 *
 * The following command line options are supported:
 *
 *      --subset=n (where `n` is an integer greater than 0)
 *          This option selects the first `n` records and displays only those.
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



