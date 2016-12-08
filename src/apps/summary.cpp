/** @page summary_table Generate the SAV Summary Table
 *
 * This application writes out summary metrics similar to the SAV summary tab.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ summary 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * The InterOp sub folder may contain any of the following files:
 *
 *  - ErrorMetricsOut.bin
 *  - ExtractionMetricsOut.bin
 *  - QMetricsOut.bin
 *  - TileMetricsOut.bin
 *
 * The run folder should have:
 *  - RunInfo.xml
 *  - RunParameters.xml (This is optional for later platforms)
 *
 * ### Error Handling
 *
 *  The `summary` program will print an error to the error stream and return an error code (any number except 0)
 *  when an error occurs. There are two likely errors that may arise:
 *
 *      1. The InterOp path was incorrect
 *      2. The InterOp files do not contain valid data
 *
 *  A missing InterOp file will be silently ignored. Incomplete InterOp files are also ignored.
 */


#include <iostream>
#include <iomanip>
#include "interop/util/math.h"
#include "interop/util/length_of.h"
#include "interop/io/metric_file_stream.h"
#include "interop/logic/summary/run_summary.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::model;
using namespace illumina::interop::model::summary;
using namespace illumina::interop::logic::summary;
using namespace illumina::interop;

/** Print the summary metrics to the given output stream
 *
 * @param out output stream
 * @param summary summary metrics
 */
void print_summary(std::ostream& out, const run_summary& summary);

int main(int argc, char** argv)
{
    const bool skip_median_calculation=true;
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    std::vector<unsigned char> valid_to_load;
    logic::utils::list_summary_metrics_to_load(valid_to_load); // Only load the InterOp files required

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;
    for(int i=1;i<argc;i++)
    {
        run_metrics run;

        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if(ret != SUCCESS) return ret;
        run_summary summary;
        try
        {
            summarize_run_metrics(run, summary, skip_median_calculation);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        try
        {
            print_summary(std::cout, summary);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    return SUCCESS;
}

/** Take a collection of strings and print them using a fixed width
 *
 * @param out output stream
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @param width of fill characters around string
 * @param fillch fill character (space by default)
 */
template<typename I>
void print_array(std::ostream& out, I beg, I end, const size_t width, const char fillch=' ')
{
    std::ios::fmtflags f( out.flags() );
    if(beg != end)
    {
        out.width(width);
        out.fill(fillch);
        out << std::left << *beg;
        ++beg;
    }
    for(;beg != end;++beg)
    {
        out << ",";
        out.width(width);
        out.fill(fillch);
        out << std::left << *beg;
    }
    out.flags(f);
    out << std::endl;
}
/** Take a array of strings and print them using a fixed width
 *
 * @param out output stream
 * @param values array of string values
 * @param width of fill characters around string
 * @param fillch fill character (space by default)
 */
template<size_t N>
void print_array(std::ostream& out, const char*(&values)[N], const size_t width, const char fillch=' ')
{
    print_array(out, values, values+N, width, fillch);
}
/** Take a array of strings and print them using a fixed width
 *
 * @param out output stream
 * @param values array of string values
 * @param width of fill characters around string
 * @param fillch fill character (space by default)
 */
template<size_t N>
void print_array(std::ostream& out, std::string(&values)[N], const size_t width, const char fillch=' ')
{
    print_array(out, values, values+N, width, fillch);
}
/** Take a vector of strings and print them using a fixed width
 *
 * @param out output stream
 * @param values vector of string values
 * @param width of fill characters around string
 * @param fillch fill character (space by default)
 */
void print_array(std::ostream& out, const std::vector<std::string>& values, const size_t width, const char fillch=' ')
{
    print_array(out, values.begin(), values.end(), width, fillch);
}
/** Format a floating point value to the given width and precision as a string
 *
 * @param val to format as a string
 * @param width width of number
 * @param precision number of values after decimal
 * @param scale to divide number
 * @return string representation of value
 */
std::string format(const float val, const int width, const int precision, const float scale=1)
{
    return util::format(val/scale, width, precision);
}
/** Format a struct of statistics to the given width and precision as a string
 *
 * @param stat struct to format as a string
 * @param width width of number
 * @param precision number of values after decimal
 * @param scale to divide number
 * @return string representation of stat
 */
std::string format(const metric_stat& stat, const int width, const int precision, const float scale=1)
{
    // TODO replace +/- with unicode: \u00B1
    // Probably requires replace std::string with a wide character string
    // Also need to check if you are on a ancient terminal
    return util::format(stat.mean()/scale, width, precision) + " +/- " + util::format(stat.stddev()/scale, width, precision);
}
/** Format cycle range as a string
 *
 * @param rng range to format as a string
 * @return string representation of cycle range
 */
std::string format(const model::run::cycle_range& rng)
{
    if(rng.first_cycle()==rng.last_cycle()) return util::lexical_cast<std::string>(rng.first_cycle());
    return util::lexical_cast<std::string>(rng.first_cycle()) + " - " + util::lexical_cast<std::string>(rng.last_cycle());
}
void summarize(const metric_summary& summary, std::vector<std::string>& values)
{
    // format(value, width in spaces, number of values after decimal, multiplier)
    size_t i=1;
    values[i++] = util::format(summary.yield_g(), 3, 2);
    values[i++] = util::format(summary.projected_yield_g(), 3, 2);
    values[i++] = util::format(summary.percent_aligned(), 3, 2);
    values[i++] = util::format(summary.error_rate(), 3, 2);
    values[i++] = util::lexical_cast<std::string>(long(summary.first_cycle_intensity()+0.5));
    values[i++] = util::format(summary.percent_gt_q30(), 3, 2);
    if(i != values.size()) INTEROP_THROW(std::runtime_error, "There is a bug in the program, columns do not match header");
}
void summarize(const surface_summary& summary, std::vector<std::string>& values, const size_t lane)
{
    size_t i=0;
    values[i++] = util::lexical_cast<std::string>(lane);
    values[i++] = util::lexical_cast<std::string>(summary.surface());
    values[i++] = util::lexical_cast<std::string>(summary.tile_count());

    // format(value, width in spaces, number of values after decimal, multiplier)
    values[i++] = format(summary.density(), 0, 0, 1e3);
    values[i++] = format(summary.percent_pf(), 0, 2);
    values[i++] = util::format(summary.phasing().mean(), 3, 3) + " / " + util::format(summary.prephasing().mean(), 3, 3);
    values[i++] = format(summary.reads(), 0, 2, 1e6);
    values[i++] = format(summary.reads_pf(), 0, 2, 1e6);
    values[i++] = format(summary.percent_gt_q30(), 0, 2);
    values[i++] = format(summary.yield_g(), 0, 2);
    values[i++] = "-";
    values[i++] = format(summary.percent_aligned(), 0, 2);
    values[i++] = format(summary.error_rate(), 0, 2);
    values[i++] = format(summary.error_rate_35(), 0, 2);
    values[i++] = format(summary.error_rate_75(), 0, 2);
    values[i++] = format(summary.error_rate_100(), 0, 2);
    values[i++] = format(summary.first_cycle_intensity(), 0, 0);
    INTEROP_ASSERT(i==values.size());
    if(i != values.size()) INTEROP_THROW(std::runtime_error, "There is a bug in the program, columns do not match header");
}
void summarize(const lane_summary& summary, std::vector<std::string>& values)
{
    size_t i=0;
    values[i++] = util::lexical_cast<std::string>(summary.lane());
    values[i++] = "-";
    values[i++] = util::lexical_cast<std::string>(summary.tile_count());

    // format(value, width in spaces, number of values after decimal, multiplier)
    values[i++] = format(summary.density(), 0, 0, 1e3);
    values[i++] = format(summary.percent_pf(), 0, 2);
    values[i++] = util::format(summary.phasing().mean(), 3, 3) + " / " + util::format(summary.prephasing().mean(), 3, 3);
    values[i++] = format(summary.reads(), 0, 2, 1e6);
    values[i++] = format(summary.reads_pf(), 0, 2, 1e6);
    values[i++] = format(summary.percent_gt_q30(), 0, 2);
    values[i++] = format(summary.yield_g(), 0, 2);
    values[i++] = format(summary.cycle_state().error_cycle_range());
    values[i++] = format(summary.percent_aligned(), 0, 2);
    values[i++] = format(summary.error_rate(), 0, 2);
    values[i++] = format(summary.error_rate_35(), 0, 2);
    values[i++] = format(summary.error_rate_75(), 0, 2);
    values[i++] = format(summary.error_rate_100(), 0, 2);
    values[i++] = format(summary.first_cycle_intensity(), 0, 0);
    INTEROP_ASSERT(i==values.size());
    if(i != values.size()) INTEROP_THROW(std::runtime_error, "There is a bug in the program, columns do not match header");
}
std::string format_read(const run::read_info& read)
{
    return "Read "+util::lexical_cast<std::string>(read.number()) + (read.is_index() ? " (I)" : "");
}

void print_summary(std::ostream& out, const run_summary& summary)
{
    const size_t width=15;
    const char* read_header[] = {"Level", "Yield", "Projected Yield", "Aligned", "Error Rate", "Intensity C1", "%>=Q30"};
    print_array(out, read_header, width);
    std::vector<std::string> values(util::length_of(read_header));
    INTEROP_ASSERT(values.size()>=1);
    for(size_t read=0;read<summary.size();++read)
    {
        values[0] = format_read(summary[read].read());
        summarize(summary[read].summary(), values);
        print_array(out, values, width);
    }
    values[0]="Non-indexed";
    summarize(summary.nonindex_summary(), values);
    print_array(out, values, width);
    values[0]="Total";
    summarize(summary.total_summary(), values);
    print_array(out, values, width);
    out<< "\n\n";

    const char* lane_header[] = {"Lane", "Surface", "Tiles", "Density", "Cluster PF", "Phas/Prephas", "Reads",
                                 "Reads PF", "%>=Q30", "Yield", "Cycles Error", "Aligned", "Error",
                                 "Error (35)", "Error (75)", "Error (100)", "Intensity C1" };
    values.resize(util::length_of(lane_header));
    for(size_t read=0;read<summary.size();++read)
    {
        out << format_read(summary[read].read()) << std::endl;
        print_array(out, lane_header, width);
        for(size_t lane=0;lane<summary.lane_count();++lane)
        {
            INTEROP_ASSERT(summary[read][lane].tile_count() > 0);
            summarize(summary[read][lane], values);
            print_array(out, values, width);
            if(summary.surface_count() > 1)
            {
                for (size_t surface = 0; surface < summary.surface_count(); ++surface)
                {
                    summarize(summary[read][lane][surface], values, summary[read][lane].lane());
                    print_array(out, values, width);
                }
            }
        }
    }
    out << "Extracted: " << format(summary.cycle_state().extracted_cycle_range()) << "\n";
    out << "Called: " << format(summary.cycle_state().called_cycle_range()) << "\n";
    out << "Scored: " << format(summary.cycle_state().qscored_cycle_range()) << "\n";
}

