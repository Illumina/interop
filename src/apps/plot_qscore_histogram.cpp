/** @page q_hist Plot the Q-score histogram
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_qscore_histogram 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-117-g05ea745-dirty
 *      # Run Folder: 1Read0Index_120423_117213Bin1R0I
 *      set terminal png nocrop
 *      set output 'q-hist.png'
 *      set style fill solid noborder
 *      set title "D0TM0ACXX All Lanes"
 *      set style data histograms
 *      set yrange [0 : 2.148 ]
 *      set xrange [1 : 38.5 ]
 *      set ylabel "Total (million)"
 *      set xlabel "Q Score"
 *      plot "-" using 1:2:3:xtic(1) with boxes notitle
 *      0       0       0
 *      10      0.0119  10
 *      20      0.003569        5
 *      25      0.036075        5
 *      30      1.95263 5
 *      0       0       0
 *      0       0       0
 *
 *
 * ### Available Options
 *
 * The following options are supported in addition to the `run folder`. They must be given in the form:
 *  `--filter-by-lane=1` with no additional space between the option name and value.
 *
 * #### Filter Options
 *
 *   - `--filter-by-lane=<lane number>`: Only the data for the selected lane will be displayed
 *   - `--filter-by-channel=<channel number>`: Only the data for the selected channel will be displayed
 *   - `--filter-by-base=<(A,C,G,T)>`: Only the data for the selected base will be displayed
 *   - `--filter-by-surface=<surface number>`: Only the data for the selected surface will be displayed
 *   - `--filter-by-read=<read number>`: Only the data for the selected read will be displayed
 *   - `--filter-by-cycle=<cycle number>`: Only the data for the selected cycle will be displayed
 *   - `--filter-by-lane=<lane number>`: Only the data for the selected lane will be displayed
 *   - `--filter-by-tile-number=<tile number>`: Only the data for the selected tile number will be displayed
 *   - `--filter-by-swath=<swath number>`: Only the data for the selected swath will be displayed
 *   - `--filter-by-section=<section number>`: Only the data for the selected section will be displayed
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/io/plot/gnuplot.h"
#include "interop/version.h"
#include "inc/application.h"
#include "inc/plot_options.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;


int main(int argc, const char** argv)
{
    if(argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        //print_help(std::cout);
        return INVALID_ARGUMENTS;
    }

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;
    model::plot::filter_options options(constants::UnknownTileNamingMethod);
    util::option_parser description;
    add_filter_options(description, options);
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
    std::vector<unsigned char> valid_to_load;
    logic::utils::list_metrics_to_load(constants::Q, valid_to_load); // Only load the InterOp files required

    for(int i=1;i<argc;i++)
    {
        run_metrics run;

        const std::string run_name = io::basename(argv[i]);
        std::cout << "# Run Folder: " << run_name << std::endl;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if(ret != SUCCESS) return ret;

        options.tile_naming_method(run.run_info().flowcell().naming_method());
        model::plot::plot_data<model::plot::bar_point> data;
        try
        {
            logic::plot::plot_qscore_histogram(run, options, data, 30u /*show the q30 boundary */);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        try
        {
            plot_writer.write_chart(out, data, plot_image_name("q-histogram", run_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

    }
    return SUCCESS;
}



