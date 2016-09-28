/** @page plot_sampleqc Plot the Indexing Graph
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_sample_qc 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-246-ge7f7de7
 *      # Run Folder: 18818805
 *      set terminal png crop
 *      set output '18818805_sample-qc.png'
 *      set yrange [0 : 14 ]
 *      set ylabel "% Reads Identified (PF)"
 *      set xrange [0 : 92 ]
 *      set xlabel "Index Number"
 *      set style fill solid border -1
 *      unset key
 *      plot "-" using 1:2:3 with boxes title "% reads" lt rgb "green"
 *      1	0.793876	1
 *      2	0.85501	1
 *      3	1.09858	1
 *      ...
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
#include "inc/plot_options.h"

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

    std::vector<unsigned char> valid_to_load;
    logic::utils::list_index_metrics_to_load(valid_to_load); // Only load the InterOp files required
    for(int i=1;i<argc;i++)
    {
        run_metrics run;

        const std::string run_name = io::basename(argv[i]);
        std::cout << "# Run Folder: " << run_name << std::endl;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
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
            plot_writer.write_chart(out, data, plot_image_name("sample-qc", run_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
    }
    return SUCCESS;
}



