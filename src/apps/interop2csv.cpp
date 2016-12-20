/** @page interop2csv Dump InterOp to a CSV text file
 *
 * The Illumina InterOp library also includes an application, which reads Illumina binary InterOp files and writes
 * the data as a special type of CSV (common separated format) file. The program takes the run folder as an input
 * and then writes the data as plain text to the console. This data can then be redirected to a file.
 *
 * @warning This program is deprecated and will be removed. Use dumptext instead
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ interop2csv 140131_1287_0851_A01n401drr
 *      $ interop2csv 140131_1287_0851_A01n401drr/InterOp
 *      $ interop2csv 140131_1287_0851_A01n401drr/InterOp/TileMetricsOut.bin
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder than contains a sub directory called InterOp, which in
 * turn may contain any of the following files:
 *
 *  - CorrectedIntMetricsOut.bin
 *  - ErrorMetricsOut.bin
 *  - ExtractionMetricsOut.bin
 *  - ImageMetricsOut.bin
 *  - IndexMetricsOut.bin
 *  - QMetricsOut.bin
 *  - TileMetricsOut.bin
 *  - QMetrics2030Out.bin
 *  - QMetricsByLaneOut.bin
 *
 *  Alternatively, you can provide a directory that contains InterOp files (e.g. 140131_1287_0851_A01n401drr/InterOp) or
 *  a specific InterOp file (e.g. 140131_1287_0851_A01n401drr/InterOp/TileMetricsOut.csv).
 *
 * The output can be redirected to a file as follows:
 *
 *      $ interop2csv 140131_1287_0851_A01n401drr > data.csv
 *
 * where data.csv contains the plain text data with the following format:
 *
 *      # 2
 *      # Tile
 *      Lane,Tile,Count,CountPF,Density,DensityPF
 *      1,12106,677812,558123,225700,185846
 *      # 2
 *      # Tile
 *      Lane,Tile,Read,Aligned,Prephasing,Phasing
 *      1,12106,1,0.772649,0.00180151,0.00253809
 *      1,12106,4,0.695119,0.00186569,0
 *      # 3
 *      # Error
 *      Lane,Tile,Cycle,Error,ReadCount,ErrorCount
 *      1,12106,1,0.437637,0,5
 *      1,12106,2,1.09409,0,5
 *      # 3
 *      # CorrectedInt
 *      Lane,Tile,Cycle,AverageCycleIntensity,SignalToNoise,CallCount_NC,CallCount_A,CallCount_C,CallCount_G,CallCount_T,CalledIntensity_A,CalledIntensity_C,CalledIntensity_G,CalledIntensity_T,AllIntensity_A,AllIntensity_C,AllIntensity_G,AllIntensity_T
 *      1,12106,1,0,0,0,163795,125981,120358,151136,2407,2787,179,2161,0,0,0,0
 *      1,12106,2,0,0,0,167773,93962,117495,182039,2480,3059,248,2278,0,0,0,0
 *      # 2
 *      # Extraction
 *      Lane,Tile,Cycle,DateTime,P90_1,P90_2,P90_3,P90_4,Focus_1,Focus_2,Focus_3,Focus_4
 *      1,12106,1,9859184553884635353,2907,2325,0,0,2.942,2.91464,0,0
 *      1,12106,2,9859184553895105659,2813,2459,0,0,2.91333,2.97001,0,0
 *      # 2
 *      # Image
 *      Lane,Tile,Cycle,ChannelCount,MinContrast_1,MinContrast_2,MaxContrast_1,MaxContrast_2
 *      1,12106,1,2,2476,2160,4693,3728
 *      1,12106,2,2,2693,2482,4922,4082
 *      # 6
 *      # Q
 *      Type,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7
 *      Lower,2,10,20,25,30,35,40
 *      Upper,9,19,24,29,34,39,40
 *      Value,2,14,21,27,32,36,40
 *      # 6
 *      # Q
 *      Lane,Tile,Cycle,BinCount,BinCount1,BinCount2,BinCount3,BinCount4,BinCount5,BinCount6,BinCount7
 *      1,12106,1,7,0,16517,8618,324,535810,0,0
 *      1,12106,2,7,0,24839,7864,690,527876,0,0
 *      # 1
 *      # Index
 *      Lane,Tile,Read,Sequence,Sample,Project,Count
 *      1,12106,3,ATCACGAC-AAGGTTCA,1,TSCAIndexes,4570
 *      1,12106,3,ACAGTGGT-AAGGTTCA,2,TSCAIndexes,4477
 *      # Version: v3.0.6-120-ga99fd3b
 *
 * ### Description of the Output
 *
 * As you can see in the example above, a metric follows the following format:
 *  1. A header prefixed with a `#` describing: the version followed by the name of an InterOp file
 *
 *          # 1
 *          # Index
 *
 *  2. Next, a column header is written
 *
 *          Lane,Tile,Read,Sequence,Sample,Project,Count
 *
 *  3. This is followed by the actual InterOp data
 *
 *          1,12106,3,ATCACGAC-AAGGTTCA,1,TSCAIndexes,4570
 *          1,12106,3,ACAGTGGT-AAGGTTCA,2,TSCAIndexes,4477
 *
 *  4. Terminated with a footer descrbing the version of the library
 *
 *          # Version: v3.0.6-120-ga99fd3b
 *
 * ### Error Handling
 *
 *  The `interop2csv` program will print an error to the error stream and return an error code (any number except 0)
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
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/enums.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop;

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
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_interops(std::ostream& out, const std::string& filename)throw(model::index_out_of_bounds_exception);

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

    // TODO: Fix up this application!
    std::cerr << "WARNING: This program is a debug utility and not currently supported. See the docs." << std::endl;

    for(int i=1;i<argc;i++)
    {
        std::cout << "# Run Folder: " << io::basename(argv[i]) << std::endl;
        try
        {
            int res = write_interops(std::cout, argv[i]);
            if (res != SUCCESS)
            {
                std::cout << "# Error: " << res << std::endl;
                std::cout << "# Version: " << INTEROP_VERSION << std::endl;
                return res;
            }
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    std::cout << "# Version: " << INTEROP_VERSION << std::endl;
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
    catch(const io::file_not_found_exception& ex){if(kPrintError) std::cerr << ex.what() << std::endl;return 1;}
    catch(const io::bad_format_exception& ex)
    {
        if(kPrintError) std::cerr << io::interop_basename<MetricSet>() << " - " << ex.what() << std::endl;
        return BAD_FORMAT;
    }
    catch(const io::incomplete_file_exception& ex){ if(kPrintError) std::cerr << ex.what() << std::endl; }
    catch(const std::exception& ex)
    {
        if(kPrintError) std::cerr << io::interop_basename<MetricSet>() << " - " << ex.what() << std::endl;
        return UNEXPECTED_EXCEPTION;
    }
    if(metrics.size()==0)
    {
        if(kPrintError) std::cerr << "Empty metric file: " << io::interop_basename<MetricSet>() << std::endl;
        return EMPTY_INTEROP;
    }
    // @ [Reading a binary InterOp file]
    return 0;
}
/** Return a new end iterator
 *
 * If kMaxRecordCount is greater than zero, then return that number of elements if it
 * is greater than the size of the vector.
 *
 * @param vec vector
 * @return end iterator
 */
template<class T>
typename T::const_iterator vec_end(const T& vec)
{
    if( kMaxRecordCount > 0 ) return vec.begin()+std::min(vec.size(),(size_t)kMaxRecordCount);
    return vec.end();
}
/** Write a standard header for each set of metrics
 *
 * @param out output stream
 * @param metrics metric set
 */
template<class MetricSet>
void write_header(std::ostream& out, const MetricSet& metrics)
{
    out << "# " << metrics.version() << "\n";
    out << "# " << io::interop_basename<MetricSet>()<< "\n";
}
/** Write a standard id for every metric derived from base metric
 *
 * @param out output stream
 * @param metric metric
 */
void write_id(std::ostream& out, const model::metric_base::base_metric& metric)
{
    out << metric.lane() << "," << metric.tile() << ",";
}
/** Write a standard id for every metric derived from base cycle metric
 *
 * @param out output stream
 * @param metric metric
 */
void write_id(std::ostream& out, const model::metric_base::base_cycle_metric& metric)
{
    out << metric.lane() << "," << metric.tile() << "," << metric.cycle() << ",";
}
/** Write a standard id for every metric derived from base read metric
 *
 * @param out output stream
 * @param metric metric
 */
void write_id(std::ostream& out, const model::metric_base::base_read_metric& metric)
{
    out << metric.lane() << "," << metric.tile() << "," << metric.read() << ",";
}

/** Write tile metrics to the output stream
 *
 * Binary File:
 *      TileMetricsOut.bin
 *
 * Format:
 *  # 2
 *  # Tile
 *   Lane,Tile,Count,CountPF,Density,DensityPF
 *  1,12106,677812,558123,225700,185846
 *  # 2
 *  # Tile
 *  Lane,Tile,Read,Aligned,Prephasing,Phasing
 *  1,12106,1,0.772649,0.00180151,0.00253809
 *  1,12106,4,0.695119,0.00186569,0
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_tile_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<tile_metric> tile_metric_set;
    tile_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Count,CountPF,Density,DensityPF\n";
    for(tile_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const tile_metric& metric = *beg;
        write_id(out, metric);
        out << metric.cluster_count() << "," << metric.cluster_count_pf() << "," << metric.cluster_density() << "," << metric.cluster_density_pf() << "\n";
    }

    write_header(out, metrics);
    out << "Lane,Tile,Read,Aligned,Prephasing,Phasing\n";
    for(tile_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const tile_metric& metric = *beg;
        for(tile_metric::read_metric_vector::const_iterator rbeg = metric.read_metrics().begin(), rend = metric.read_metrics().end();rbeg != rend;++rbeg)
        {
            write_id(out, metric);
            out << rbeg->read() << "," << rbeg->percent_aligned() << "," << rbeg->percent_prephasing() << "," << rbeg->percent_phasing() << "\n";
        }
    }

    return res;
}

/** Write error metrics to the output stream
 *
 * Binary File:
 *      ErrorMetricsOut.bin
 *
 * Format:
 *  # 3
 *  # Error
 *  Lane,Tile,Cycle,Error,ReadCount,ErrorCount
 *  1,12106,1,0.437637,0,5
 *  1,12106,2,1.09409,0,5
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_error_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<error_metric> error_metric_set;
    error_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,Error\n";
    for(error_metric_set::metric_array_t::const_iterator beg = metrics.metrics().begin(), end = vec_end(metrics.metrics());beg != end;++beg)
    {
        const error_metric& metric = *beg;
        write_id(out, metric);
        out << metric.error_rate() << "\n";
    }

    return res;
}

/** Write corrected intensity metrics to the output stream
 *
 * Binary File:
 *      CorrectedIntMetricsOut.bin
 *
 * Format:
 *  # 3
 *  # CorrectedInt
 *  Lane,Tile,Cycle,AverageCycleIntensity,SignalToNoise,CallCount_NC,CallCount_A,CallCount_C,CallCount_G,CallCount_T,CalledIntensity_A,CalledIntensity_C,CalledIntensity_G,CalledIntensity_T,AllIntensity_A,AllIntensity_C,AllIntensity_G,AllIntensity_T
 *  1,12106,1,0,0,0,163795,125981,120358,151136,2407,2787,179,2161,0,0,0,0
 *  1,12106,2,0,0,0,167773,93962,117495,182039,2480,3059,248,2278,0,0,0,0
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_corrected_intensity_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<corrected_intensity_metric> corrected_intensity_metric_set;
    corrected_intensity_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,AverageCycleIntensity,SignalToNoise,";
    out << "CallCount_NC,CallCount_A,CallCount_C,CallCount_G,CallCount_T,";
    out << "CalledIntensity_A,CalledIntensity_C,CalledIntensity_G,CalledIntensity_T,";
    out << "AllIntensity_A,AllIntensity_C,AllIntensity_G,AllIntensity_T";
    out << "\n";
    for(corrected_intensity_metric_set::metric_array_t::const_iterator beg = metrics.metrics().begin(), end = vec_end(metrics.metrics());beg != end;++beg)
    {
        const corrected_intensity_metric& metric = *beg;
        write_id(out, metric);
        out << metric.average_cycle_intensity() << "," << metric.signal_to_noise();
        for(int i=-1;i<constants::NUM_OF_BASES;i++)
            out << "," << metric.called_counts(static_cast<constants::dna_bases>(i));
        for(size_t i=0;i<constants::NUM_OF_BASES;i++)
            out << "," << metric.corrected_int_called(static_cast<constants::dna_bases>(i));
        for(size_t i=0;i<constants::NUM_OF_BASES;i++)
            out << "," << metric.corrected_int_all(static_cast<constants::dna_bases>(i));
        out << "\n";
    }

    return res;
}


/** Write extraction metrics to the output stream
 *
 * Binary File:
 *      ExtractionMetricsOut.bin
 *
 * Format:
 *  # 2
 *  # Extraction
 *  Lane,Tile,Cycle,DateTime,P90_1,P90_2,P90_3,P90_4,Focus_1,Focus_2,Focus_3,Focus_4
 *  1,12106,1,9859184553884635353,2907,2325,0,0,2.942,2.91464,0,0
 *  1,12106,2,9859184553895105659,2813,2459,0,0,2.91333,2.97001,0,0
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_extraction_metrics(std::ostream& out, const std::string& filename) throw(model::index_out_of_bounds_exception)
{
    typedef model::metric_base::metric_set<extraction_metric> extraction_metric_set;
    extraction_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,DateTime,";
    out << "P90_1,P90_2,P90_3,P90_4,";
    out << "Focus_1,Focus_2,Focus_3,Focus_4";
    out << "\n";
    for(extraction_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const extraction_metric& metric = *beg;
        write_id(out, metric);
        out  << metric.date_time();
        for(size_t i=0;i<extraction_metric::MAX_CHANNELS;i++)
            out << "," << metric.max_intensity(i);
        for(size_t i=0;i<extraction_metric::MAX_CHANNELS;i++)
            out << "," << metric.focus_score(i);
        out << "\n";
    }

    return res;
}
/** Write image metrics to the output stream
 *
 * Binary File:
 *      ImageMetricsOut.bin
 *
 * Format:
 *  # 2
 *  # Image
 *  Lane,Tile,Cycle,ChannelCount,MinContrast_1,MinContrast_2,MaxContrast_1,MaxContrast_2
 *  1,12106,1,2,2476,2160,4693,3728
 *  1,12106,2,2,2693,2482,4922,4082
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_image_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<image_metric> image_metric_set;
    image_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,ChannelCount";
    for(size_t i=0;i<metrics.channel_count();i++)
        out << ",MinContrast_" << i+1;
    for(size_t i=0;i<metrics.channel_count();i++)
        out << ",MaxContrast_" << i+1;
    out << "\n";
    for(image_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const image_metric& metric = *beg;
        write_id(out, metric);
        out  << metric.channel_count();
        for(size_t i=0;i<metric.channel_count();i++)
            out << "," << metric.min_contrast(i);
        for(size_t i=0;i<metric.channel_count();i++)
            out << "," << metric.max_contrast(i);
        out << "\n";
    }

    return res;
}
/** Write collapsed q-metrics to the output stream
 *
 * Binary File:
 *      QMetrics2030Out.bin
 *
 * Format:
 *  # 6
 *  # Q2030
 *  Lane,Tile,Cycle,Q20,Q30,Total,MedianQScore
 *  1,1105,1,2447414,2334829,2566750,33
 *  1,1103,1,2436317,2327796,2543605,33
 *  1,1106,1,2474217,2366046,2583629,33
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_collapsed_q_metrics(std::ostream& out, const std::string& filename)
{
    typedef metric_set<q_collapsed_metric> q_collapsed_metric_set;
    typedef q_collapsed_metric_set::const_iterator const_iterator;
    q_collapsed_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,Q20,Q30,Total,MedianQScore\n";
    for(const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const q_collapsed_metric& metric = *beg;
        write_id(out, metric);
        out << metric.q20() <<"," << metric.q30() << "," << metric.total() << "," << metric.median_qscore() << "\n";
    }
    return res;
}

/** Write q-metrics to the output stream
 *
 * Binary File:
 *      QMetricsOut.bin
 *
 * Format:
 *  # 6
 *  # Q
 *  Type,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7
 *  Lower,2,10,20,25,30,35,40
 *  Upper,9,19,24,29,34,39,40
 *  Value,2,14,21,27,32,36,40
 *  # 6
 *  # Q
 *  Lane,Tile,Cycle,BinCount,BinCount1,BinCount2,BinCount3,BinCount4,BinCount5,BinCount6,BinCount7
 *  1,12106,1,7,0,16517,8618,324,535810,0,0
 *  1,12106,2,7,0,24839,7864,690,527876,0,0
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_q_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<q_metric> q_metric_set;
    q_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    if(metrics.bin_count()>0)
    {
        write_header(out, metrics);
        out << "Type";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << ",Bin" << i + 1;
        out << "\n";
        out << "Lower";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).lower();
        out << "\n";
        out << "Upper";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).upper();
        out << "\n";
        out << "Value";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).value();
        out << "\n";
    }

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,BinCount";
    const size_t hist_bin_count = logic::metric::count_q_metric_bins(metrics);
    for(size_t i=0;i<hist_bin_count;++i)
        out << ",BinCount" << i+1;
    out << "\n";
    for(q_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const q_metric& metric = *beg;
        write_id(out, metric);
        out  << metric.size();
        for(size_t i=0;i<metric.size();i++)
            out << "," << metric.qscore_hist(i);
        out << "\n";
    }

    return res;
}


/** Write q-metrics by lane to the output stream
 *
 * Binary File:
 *      QMetricsByLaneOut.bin
 *
 * Format:
 *  # 6
 *  # Q
 *  Type,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7
 *  Lower,2,10,20,25,30,35,40
 *  Upper,9,19,24,29,34,39,40
 *  Value,2,14,21,27,32,36,40
 *  # 6
 *  # Q
 *  Lane,Tile,Cycle,BinCount,BinCount1,BinCount2,BinCount3,BinCount4,BinCount5,BinCount6,BinCount7
 *  1,0,1,7,0,16517,8618,324,535810,0,0
 *  1,0,2,7,0,24839,7864,690,527876,0,0
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_q_by_lane_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<q_by_lane_metric> q_metric_set;
    q_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    if(metrics.bin_count()>0)
    {
        write_header(out, metrics);
        out << "Type";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << ",Bin" << i + 1;
        out << "\n";
        out << "Lower";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).lower();
        out << "\n";
        out << "Upper";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).upper();
        out << "\n";
        out << "Value";
        for (size_t i = 0; i < metrics.bin_count(); i++)
            out << "," << metrics.bin_at(i).value();
        out << "\n";
    }

    write_header(out, metrics);
    out << "Lane,Tile,Cycle,BinCount";
    const size_t hist_bin_count = logic::metric::count_q_metric_bins(metrics);
    for(size_t i=0;i<hist_bin_count;++i)
        out << ",BinCount" << i+1;
    out << "\n";
    for(q_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const q_metric& metric = *beg;
        write_id(out, metric);
        out  << metric.size();
        for(size_t i=0;i<metric.size();i++)
            out << "," << metric.qscore_hist(i);
        out << "\n";
    }

    return res;
}

/** Write index metrics to the output stream
 *
 * Binary File:
 *      IndexMetricsOut.bin
 *
 * Format:
 *  # 1
 *  # Index
 *  Lane,Tile,Read,Sequence,Sample,Project,Count
 *  1,12106,3,ATCACGAC-AAGGTTCA,1,TSCAIndexes,4570
 *  1,12106,3,ACAGTGGT-AAGGTTCA,2,TSCAIndexes,4477
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_index_metrics(std::ostream& out, const std::string& filename)
{
    typedef model::metric_base::metric_set<index_metric> index_metric_set;
    index_metric_set metrics;
    int res = read_metrics_from_file(filename, metrics);
    if(res != 0) return res;

    write_header(out, metrics);
    out << "Lane,Tile,Read,Sequence,Sample,Project,ClusterCount\n";
    for(index_metric_set::metric_array_t::const_iterator beg = metrics.begin(), end = vec_end(metrics);beg != end;++beg)
    {
        const index_metric& metric = *beg;
        for(index_metric::index_array_t::const_iterator index_beg=metric.indices().begin(), index_end=metric.indices().end();index_beg != index_end;++index_beg)
        {
            write_id(out, metric);
            out << index_beg->index_seq() << "," << index_beg->sample_id() << "," << index_beg->sample_proj() << "," << index_beg->cluster_count() << "\n";
        }
    }

    return res;
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
 *  8. Collapsed Q
 *  9. By Lane Q
 *
 * @param out output stream
 * @param filename path to run folder
 * @return error code or 0
 */
int write_interops(std::ostream& out, const std::string& filename) throw(model::index_out_of_bounds_exception)
{
    int res;
    int valid_count = 0;
    if((res=write_tile_metrics(out, filename)) > 1) return encode_error(res, 1);
    if(res == 0) valid_count++;
    if((res=write_error_metrics(out, filename)) > 1) return encode_error(res, 2);
    if(res == 0) valid_count++;
    if((res=write_corrected_intensity_metrics(out, filename)) > 1) return encode_error(res, 3);
    if(res == 0) valid_count++;
    if((res=write_extraction_metrics(out, filename)) > 1) return encode_error(res, 4);
    if(res == 0) valid_count++;
    if((res=write_image_metrics(out, filename)) > 1) return encode_error(res, 5);
    if(res == 0) valid_count++;
    if((res=write_q_metrics(out, filename)) > 1) return encode_error(res, 6);
    if(res == 0) valid_count++;
    if((res=write_index_metrics(out, filename)) > 1) return encode_error(res, 7);
    if(res == 0) valid_count++;
    if((res=write_collapsed_q_metrics(out, filename)) > 1) return encode_error(res, 8);
    if(res == 0) valid_count++;
    if((res=write_q_by_lane_metrics(out, filename)) > 1) return encode_error(res, 9);
    if(res == 0) valid_count++;

    if(valid_count == 0)
    {
        if(kPrintError) std::cerr << "No files found" << std::endl;
        return EMPTY_INTEROP;
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
    out << "Usage: interop2csv run-folder" << std::endl;
}


