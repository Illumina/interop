/** @page apps Applications
 *
 * @brief Write out InterOps for all entries up to a certain cycle
 *
 * This application writes out a new set of binary InterOp files for all records up to a specific cycle.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ cyclesim 140131_1287_0851_A01n401drr 26
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder than contains a sub directory called InterOp and 26
 * is the maximum number of cycles for the records. The program will output a directory called
 * 140131_1287_0851_A01n401drr_MaxCycle_26
 *
 * The InterOp sub folder may contain any of the following files:
 *
 *  - CorrectedIntMetricsOut.bin
 *  - ErrorMetricsOut.bin
 *  - ExtractionMetricsOut.bin
 *  - ImageMetricsOut.bin
 *  - IndexMetricsOut.bin
 *  - QMetricsOut.bin
 *  - TileMetricsOut.bin
 *
 * Error Handling
 * --------------
 *
 *  The `cyclesim` program will print an error to the error stream and return an error code (any number except 0)
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
#include "interop/util/lexical_cast.h"
#include "interop/model/metric_sets/tile_metric_set.h"
#include "interop/model/metric_sets/error_metric_set.h"
#include "interop/model/metric_sets/corrected_intensity_metric_set.h"
#include "interop/model/metric_sets/extraction_metric_set.h"
#include "interop/model/metric_sets/image_metric_set.h"
#include "interop/model/metric_sets/q_metric_set.h"
#include "interop/version.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

/** Exit codes that can be produced by the application
 */
enum exit_codes
{
    /** The program exited cleanly, 0 */
            SUCCESS,
    /** Invalid arguments were given to the application*/
            INVALID_ARGUMENTS,
    /** Empty InterOp directory*/
            NO_INTEROPS_FOUND,
    /** InterOp file has a bad format */
            BAD_FORMAT,
    /** Unknown error has occurred*/
            UNEXPECTED_EXCEPTION,
    /** InterOp file has not records */
            EMPTY_INTEROP
};
/** Write a help message to the output stream
 *
 * @param out output stream
 */
void print_help(std::ostream& out);
/** Read all the metrics and write them to the output stream
 *
 * This reads metrics from the binary interop and writes to the output stream in the following order:
 *  1. Tile
 *  2. Error
 *  3. Corrected Intensity
 *  4. Extraction
 *  5. Image
 *  6. Q
 *  7. Index
 *
 * @param input path to run folder
 * @param output path to output run folder
 * @param max_cycle number of cycles to copy
 * @return error code or 0
 */
int write_interops(const std::string& input, const std::string& output, unsigned int max_cycle);

/** Set false if you want to disable error messages printing to the error stream */
bool kPrintError=true;
/** Set greater than zero if you want to view less recoreds */
int kMaxRecordCount=0;


int main(int argc, char** argv)
{
    if(argc == 0)
    {
        if(kPrintError) std::cerr << "No arguments specified!" << std::endl;
        if(kPrintError) print_help(std::cout);
        return 1;
    }
    if(argc == 1)
    {
        if(kPrintError) std::cerr << "Too few arguments specified!" << std::endl;
        if(kPrintError) print_help(std::cout);
        return 1;
    }

    std::cout << "Cycle Simulator " << INTEROP_VERSION << std::endl;
    std::string run_folder = argv[1];
    std::string output_folder = run_folder + MaxCycle + argv[2];
    unsigned int max_cycle = util::lexical_cast<unsigned int>(argv[2]);
    int res = write_interops(run_folder, output_folder, max_cycle);
    if(res != SUCCESS)
    {
        std::cout << "# Error: " << res << std::endl;
        std::cout << "# Version: " << INTEROP_VERSION << std::endl;
        return res;
    }
    return SUCCESS;
}

/** Read metric from a file
 *
 * This function turns expected exceptions into return codes
 *
 * @param filename path to run folder
 * @param metrics metric set
 * @return error code or 0
 */
template<class MetricSet>
int read_metrics_from_file(const std::string& filename, MetricSet& metrics)
{
    // @ [Reading a binary InterOp file]
    try {
        io::read_interop(filename, metrics);
    }
    catch(const io::file_not_found_exception&){return 1;}
    catch(const io::bad_format_exception& ex)
    {
        if(kPrintError) std::cerr << ex.what() << std::endl;
        return BAD_FORMAT;
    }
    catch(const io::incomplete_file_exception&){ }
    catch(const std::exception& ex)
    {
        if(kPrintError) std::cerr << ex.what() << std::endl;
        return UNEXPECTED_EXCEPTION;
    }
    if(metrics.size()==0)
    {
        if(kPrintError) std::cerr << "Empty metric file: " << metrics.name() << std::endl;
        return EMPTY_INTEROP;
    }
    // @ [Reading a binary InterOp file]
    return 0;
}

int copy_tile_metrics(const std::string& input, const std::string& output, unsigned int max_cycle)
{
    tile_metrics metrics;
    int res;
    if((res=read_metrics_from_file(input, metrics)) != 0) return res;

    std::cout << metrics.name() << ": " << metrics.version() << std::endl;
    return 0;
}

template<class MetricSet>
int copy_cycle_metrics(const std::string& input, const std::string& output, unsigned int max_cycle)
{
    MetricSet metrics;
    int res;
    if((res=read_metrics_from_file(input, metrics)) != 0) return res;

    std::cout << metrics.name() << ": " << metrics.version() << std::endl;
    return 0;
}

/** Encode error type and metric type into a single code
 *
 * @param res type of the error
 * @param type type of the metric
 * @return combined code
 */
int encode_error(int res, int type)
{
    return res*100 + type;
}

int write_interops(const std::string& filename, const std::string& output, unsigned int max_cycle)
{
    int res;
    int valid_count = 0;
    if((res=copy_tile_metrics(out, filename, max_cycle)) > 1) return encode_error(res, 1);
    if(res == 0) valid_count++;
    if((res=copy_cycle_metrics<error_metrics>(out, filename, max_cycle)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if((res=copy_cycle_metrics<corrected_intensity_metrics>(out, filename, max_cycle)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if((res=copy_cycle_metrics<extraction_metrics>(out, filename, max_cycle)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if((res=copy_cycle_metrics<q_metrics>(out, filename, max_cycle)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if((res=copy_cycle_metrics<image_metrics>(out, filename, max_cycle)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if(valid_count == 0)
    {
        if(kPrintError) std::cerr << "No files found" << std::endl;
        return NO_INTEROPS_FOUND;
    }
    return 0;
}

/** Write a help message to the output stream
 *
 * @param out output stream
 */
void print_help(std::ostream& out)
{
    out << "Version: " << INTEROP_VERSION << std::endl;
    out << "Usage: cyclesim run-folder cycle-count" << std::endl;
}