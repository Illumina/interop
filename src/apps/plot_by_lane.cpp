/** @page plot_by_lane Plot by lane
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_by_lane 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-147-gb6d5c19-dirty
 *      # Run Folder: 11115124_11854Unbin2R2I
 *      set terminal png nocrop
 *      set output 'plot_by_lane.png'
 *      set title "000000000-ABWLV"
 *      set yrange [0 : 1528.36 ]
 *      set ylabel "Density (K/mm2)"
 *      set xrange [0 : 2 ]
 *      set xlabel "Lane"
 *      plot "-" using 1:3:2:6:5 with candlesticks title "Density (K/mm2)" lt rgb "blue" whiskerbars ,"-" using 1:3:2:6:5 with candlesticks title "PF" lt rgb "dark-green" whiskerbars
 *      1	1164.6	1205.49	1218.97	1232.75	1273.63
 *      e
 *      1	955.386	1015.23	1036.47	1055.12	1114.97
 *      e
 *
 * ### Available Options
 *
 * The following options are supported in addition to the `run folder`. They must be given in the form:
 *  `--filter-by-lane=1` with no additional space between the option name and value.
 *
 * #### Metric Options
 *
 *   - `--metric-name=<name>`: Metric to plot
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
#include "interop/logic/plot/plot_by_lane.h"
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
    std::string metric_name="ClusterCount";
    util::option_parser description;
    add_metric_option(description, metric_name);
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
    try{
        logic::utils::list_metrics_to_load(metric_name, valid_to_load); // Only load the InterOp files required
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return INVALID_ARGUMENTS;
    }

    for(int i=1;i<argc;i++)
    {
        run_metrics run;

        const std::string run_name = io::basename(argv[i]);
        std::cout << "# Run Folder: " << run_name << std::endl;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if(ret != SUCCESS) return ret;

        options.tile_naming_method(run.run_info().flowcell().naming_method());

        model::plot::plot_data<model::plot::candle_stick_point> data;
        try
        {
            logic::plot::plot_by_lane(run, metric_name, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        try{
            plot_writer.write_chart(out, data, plot_image_name(metric_name+"-by-lane", run_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    return SUCCESS;
}



