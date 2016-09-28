/** @page q_hmap Plot the Q-score Heat map
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_qscore_heatmap 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-147-gc04a08b
 *      # Run Folder: 131212_221Bin1R0I
 *      set terminal png nocrop
 *      set output 'q_heatmap.png'
 *      set title "02D224DRR All Lanes"
 *      set yrange [0 : 45 ]
 *      set ylabel "Q Score"
 *      set xrange [0 : 31 ]
 *      set xlabel "Cycle"
 *      set view map
 *      plot "-" matrix with image
 *      0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 *      0 0 0 0 0 0 0.017353 0.017634 0.0162293 0.019017 0.0131606 0.0134416 0.0162293 0.024895 0.0209836 0.0229501 0.0137009 0.019017 0.0162293 0.00922758 0.0243331 0 0.0380556 0.042248 0.0551277 0.0355272 0.0433718 0.437348 0.261354 1.08708 0
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
#include "interop/logic/plot/plot_qscore_heatmap.h"
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

    run_metrics run;
    for(int i=1;i<argc;i++)
    {
        run.clear();
        const std::string run_name = io::basename(argv[i]);
        std::cout << "# Run Folder: " << run_name << std::endl;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if(ret != SUCCESS) return ret;


        options.tile_naming_method(run.run_info().flowcell().naming_method());
        model::plot::heatmap_data data;
        try
        {
            logic::plot::plot_qscore_heatmap(run, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        if(data.length() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        try
        {
            plot_writer.write_heatmap(out, data, plot_image_name("q-heat-map", run_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

    }
    return SUCCESS;
}



