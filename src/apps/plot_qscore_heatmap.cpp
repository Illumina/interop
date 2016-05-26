/** @page apps Applications
 *
 * @brief Write a Q-score heat map to the console as at TSV (tab separated values) with a GNUPlot header
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ plot_qscore_heatmap 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * # Version: v1.0.4-147-gc04a08b
 * # Run Folder: 131212_221Bin1R0I
 * set terminal png nocrop
 * set output 'q_heatmap.png'
 * set title "02D224DRR All Lanes"
 * set yrange [0 : 45 ]
 * set ylabel "Q Score"
 * set xrange [0 : 31 ]
 * set xlabel "Cycle"
 * set view map
 * plot "-" matrix with image
 * 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0.017353 0.017634 0.0162293 0.019017 0.0131606 0.0134416 0.0162293 0.024895 0.0209836 0.0229501 0.0137009 0.019017 0.0162293 0.00922758 0.0243331 0 0.0380556 0.042248 0.0551277 0.0355272 0.0433718 0.437348 0.261354 1.08708 0
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
        plot_writer.write_heatmap(out, data, "q_heatmap.png");

    }
    return SUCCESS;
}


