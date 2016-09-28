/** @page index_summary Generate the SAV Indexing Table
 *
 *
 * This application writes out index summary metrics similar to the SAV indexing tab.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ index-summary 11115124_11854Unbin2R2I
 *
 * In this sample, 11115124_11854Unbin2R2I is a run folder and the summary is written to the standard output
 * as follows
 *
 *      # Version: v1.0.4-224-gacc6c8e
 *      Lane 1
 *      Total Reads     PF Reads        % Read Identified (PF) CV              Min             Max
 *      22855008        19391826        96.7232         0.5136          0.0001          2.9831
 *      Index Number    Sample Id       Project         Index 1 (I7)    Index 2 (I5)    % Read Identified (PF)
 *      1               AF_1-5_spike    NA              AAGAGGCA        AAGGAGTA        1.2232
 *      2               WU_1_spike      NA              AAGAGGCA        ACTGCATA        1.5316
 *      ...
 *
 * The InterOp sub folder may contain any of the following files:
 *
 *  - IndexMetricsOut.bin
 *  - TileMetricsOut.bin
 *
 * The run folder should have:
 *  - RunInfo.xml
 *  - RunParameters.xml (This is optional for later platforms)
 *
 * ### Error Handling
 *
 *  The `index-summary` program will print an error to the error stream and return an error code (any number except 0)
 *  when an error occurs. There are two likely errors that may arise:
 *
 *      1. The InterOp path was incorrect
 *      2. The InterOp files do not contain valid data
 *
 *  A missing InterOp file will be silently ignored. Incomplete InterOp files are also ignored.
 */


#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/logic/summary/index_summary.h"
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
void print_summary(std::ostream& out, const index_flowcell_summary& summary);

int main(int argc, char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    std::vector<unsigned char> valid_to_load;
    logic::utils::list_index_metrics_to_load(valid_to_load); // Only load the InterOp files required
    std::cout << "# Version: " << INTEROP_VERSION << std::endl;
    for(int i=1;i<argc;i++)
    {
        run_metrics run;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if (ret != SUCCESS) return ret;
        index_flowcell_summary summary;
        try
        {
            summarize_index_metrics(run, summary);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        summary.sort();
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
    for(;beg != end;++beg)
    {
        out << " ";
        out.width(width);
        out.fill(fillch);
        out << std::left << *beg;
    }
    out.flags(f);
    out << std::endl;
}
/** Get number of elements in stack array
 *
 * @return number of elements
 */
template<size_t N>
size_t size_of(const char*(&)[N])
{
    return N;
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

std::string format_read(const run::read_info& read)
{
    return "Read "+util::lexical_cast<std::string>(read.number()) + (read.is_index() ? " (I)" : "");
}

void populate_index(const index_count_summary& summary, std::vector<std::string>& values)
{
    values[0] = format(static_cast<float>(summary.id()), 0, 0);
    values[1] = summary.sample_id();
    values[2] = summary.project_name();
    values[3] = summary.index1();
    values[4] = summary.index2();
    values[5] = format(summary.fraction_mapped(), 0, 4);
}

void print_summary(std::ostream& out, const index_lane_summary& summary)
{
    const size_t width=15;
    const char* flowcell_header[] = {"Total Reads", "PF Reads", "% Read Identified (PF)", "CV", "Min", "Max"};
    print_array(out, flowcell_header, width);
    std::vector<std::string> values(size_of(flowcell_header));
    values[0] = format(static_cast<float>(summary.total_reads()), 0, 0);
    values[1] = format(static_cast<float>(summary.total_pf_reads()), 0, 0);
    values[2] = format(summary.total_fraction_mapped_reads(), 0, 4);
    values[3] = format(summary.mapped_reads_cv(), 0, 4);
    values[4] = format(summary.min_mapped_reads(), 0, 4);
    values[5] = format(summary.max_mapped_reads(), 0, 4);
    print_array(out, values, width);

    const char* index_header[] = {"Index Number", "Sample Id", "Project", "Index 1 (I7)", "Index 2 (I5)", "% Read Identified (PF)"};
    values.resize(size_of(index_header));
    print_array(out, index_header, width);
    for(size_t index=0;index<summary.size();++index)
    {
        populate_index(summary[index], values);
        print_array(out, values, width);
    }
}

void print_summary(std::ostream& out, const index_flowcell_summary& summary)
{
    for(size_t lane=0;lane<summary.size();++lane)
    {
        out << "Lane " << lane+1 << "\n";
        print_summary(out, summary[lane]);
    }
}


