/** @page apps Applications
 *
 * @brief Write sample QC plot to the console as at TSV (tab separated values) with a GNUPlot header
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ plot_sample_qc 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 * # Version: v1.0.4-147-gb6d5c19-dirty
 * # Run Folder: 11115124_11854Unbin2R2I
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_sample_qc.h"
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

        model::plot::plot_data<model::plot::bar_point> data;
        try
        {
            for(size_t lane=1;lane<=run.run_info().flowcell().lane_count();++lane)
                logic::plot::plot_sample_qc(run, lane, data);
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
            plot_writer.write_chart(out, data, "plot_by_lane.png");
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    return SUCCESS;
}


