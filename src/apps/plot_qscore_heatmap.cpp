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
#include "interop/logic/plot/plot_qscore_heatmap.h"
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
                                            model::plot::filter_options::ALL_CHANNELS,
                                            (constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            //,1
                                            );
        model::plot::heatmap_data data;
        logic::plot::plot_qscore_heatmap(run, options, data);
        if(data.length() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_heatmap(out, data, "q_heatmap.png");

    }
    return SUCCESS;
}


