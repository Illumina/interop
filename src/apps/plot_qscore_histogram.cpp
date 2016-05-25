/** @page apps Applications
 *
 * @brief Write a Q-score histogram to the console as at TSV (tab separated values) with a GNUPlot header
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ plot_qscore_histogram 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * # Version: v1.0.4-117-g05ea745-dirty
 * # Run Folder: 1Read0Index_120423_117213Bin1R0I
 * set terminal png nocrop
 * set output 'q-hist.png'
 * set style fill solid noborder
 * set title "D0TM0ACXX All Lanes"
 * set style data histograms
 * set yrange [0 : 2.148 ]
 * set xrange [1 : 38.5 ]
 * set ylabel "Total (million)"
 * set xlabel "Q Score"
 * plot "-" using 1:2:3:xtic(1) with boxes notitle
 * 0       0       0
 * 10      0.0119  10
 * 20      0.003569        5
 * 25      0.036075        5
 * 30      1.95263 5
 * 0       0       0
 * 0       0       0
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
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


int main(int argc, char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;

    for(int i=1;i<argc;i++)
    {
        run_metrics run;
        try
        {
            std::cout << "# Run Folder: " << io::basename(argv[i]) << std::endl;
            run.read(argv[i]);
        }
        catch(const io::bad_format_exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return BAD_FORMAT;
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        if(run.empty())
        {
            std::cerr << "No InterOp files found" << std::endl;
            return EMPTY_INTEROP;
        }


        model::plot::filter_options options(run.run_info().flowcell().naming_method(),
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_CHANNELS,
                                            (constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            //,1
                                            );
        model::plot::plot_data<model::plot::bar_point> data;
        logic::plot::plot_qscore_histogram(run, options, data);
        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        // -----------------------------------------
        // GNUPlot compatible header
        // -----------------------------------------
        out << "set terminal png nocrop" << std::endl;
        out << "set output \'q-hist.png\'" << std::endl;
        out << "set style fill solid noborder" << std::endl;
        out << "set title \"" << data.title() << "\"" << std::endl;
        out << "set style data histograms" << std::endl;
        out << "set yrange [" << data.y_axis().min() << " : " << data.y_axis().max() << " ]" << std::endl;
        out << "set xrange [" << data.x_axis().min() << " : " << data.x_axis().max() << " ]" << std::endl;
        out << "set ylabel \"" << data.y_axis().label() << "\"" << std::endl;
        out << "set xlabel \"" << data.x_axis().label() << "\"" << std::endl;
        out << "plot \"-\" using 1:2:3:xtic(1) with boxes notitle" << std::endl;
        // -----------------------------------------
        const model::plot::data_point_collection<model::plot::bar_point>& points = data[0];
        // Tab separated values
        for(size_t i=0;i<points.size();++i)
        {
            out << points[i].x() << "\t" << points[i].y() << "\t" << points[i].width() << std::endl;
        }

    }
    return SUCCESS;
}


