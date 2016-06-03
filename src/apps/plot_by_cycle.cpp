/** @page apps Applications
 *
 * @brief Write metric by cycle plot to the console as at TSV (tab separated values) with a GNUPlot header
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ plot_by_cycle 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * # Version: v1.0.4-147-gb6d5c19-dirty
 * # Run Folder: 1177190_11618Unbin2R1I
 * set terminal png nocrop
 * set output 'plot_by_cycle.png'
 * set title "000000000-A1W6M All Lanes Channel A All Surfaces"
 * set yrange [0 : 850.85 ]
 * set ylabel "Intensity"
 * set xrange [0 : 80 ]
 * set xlabel "Cycle"
 * plot "-" using 1:3:2:6:5 with candlesticks notitle lt rgb "black" whiskerbars
 * 1	98.5	106	109	111	118.5
 * 2	83.5	103	110	116	135.5
 * 3	87	102	107	112	127
 * e
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_by_cycle.h"
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
        constants::G, //(constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            //,1
                                            );

        model::plot::plot_data<model::plot::candle_stick_point> data;

        try{
            logic::plot::plot_by_cycle(run, constants::CorrectedIntensity, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_chart(out, data, "plot_by_cycle.png");
    }
    return SUCCESS;
}


