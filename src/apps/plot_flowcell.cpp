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
 * # Version: v1.0.4-117-g05ea745-dirty
 * # Run Folder: 1Read0Index_120423_117213Bin1R0I
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_flowcell_map.h"
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
                                            (constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            ,1
                                            );

        model::plot::flowcell_data data;
        try
        {
            logic::plot::plot_flowcell_map(run, "Intensity", options, data);
        }
        catch(const std::invalid_argument& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.length() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_flowcell(out, data, "flowcell.png");
        //plot_writer.write_flowcell_tile_id(out, data, "flowcell.png");
    }
    return SUCCESS;
}


