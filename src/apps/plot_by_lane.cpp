/** @page apps Applications
 *
 * @brief Write metric by lane plot to the console as at TSV (tab separated values) with a GNUPlot header
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ plot_by_lane 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * # Version: v1.0.4-147-gb6d5c19-dirty
 * # Run Folder: 11115124_11854Unbin2R2I
 * set terminal png nocrop
 * set output 'plot_by_lane.png'
 * set title "000000000-ABWLV"
 * set yrange [0 : 1528.36 ]
 * set ylabel "Density (K/mm2)"
 * set xrange [0 : 2 ]
 * set xlabel "Lane"
 * plot "-" using 1:3:2:6:5 with candlesticks title "Density (K/mm2)" lt rgb "blue" whiskerbars ,"-" using 1:3:2:6:5 with candlesticks title "PF" lt rgb "dark-green" whiskerbars
 * 1	1164.6	1205.49	1218.97	1232.75	1273.63
 * e
 * 1	955.386	1015.23	1036.47	1055.12	1114.97
 * e
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/io/plot/gnuplot.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

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

        std::cout << "# Run Folder: " << io::basename(argv[i]) << std::endl;
        int ret = read_run_metrics(argv[i], run);
        if(ret != SUCCESS) return ret;


        model::plot::filter_options options(run.run_info().flowcell().naming_method(),
                                            model::plot::filter_options::ALL_IDS,
                                            0,
                //model::plot::filter_options::ALL_CHANNELS,
                                            (constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            //,1
                                            );

        model::plot::plot_data<model::plot::candle_stick_point> data;
        try
        {
            logic::plot::plot_by_lane(run, constants::ClusterCount, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_chart(out, data, "plot_by_lane.png");
    }
    return SUCCESS;
}


