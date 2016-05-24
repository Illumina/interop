/** @page apps Applications
 *
 * @brief Write a Q-score histogram to the console as a CSV file
 *
 * This application
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
 * # Version: v1.0.4-70-g9bcfb5a-dirty
 * set title ""
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/version.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

/** Exit codes that can be produced by the application
 */
enum exit_codes
{
    /** The program exited cleanly, 0 */
    SUCCESS,
    /** Invalid arguments were given to the application*/
    INVALID_ARGUMENTS,
    /** Empty InterOp directory*/
    NO_INTEROPS_FOUND,
    /** InterOp file has a bad format */
    BAD_FORMAT,
    /** Unknown error has occurred*/
    UNEXPECTED_EXCEPTION,
    /** InterOp file has not records */
    EMPTY_INTEROP
};


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
        try
        {
            std::cout << "# Run Folder: " << io::basename(argv[i]) << std::endl;
            run.read(argv[i]);
        }
        catch(const io::bad_format_exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return BAD_FORMAT;
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        if(run.empty())
        {
            std::cerr << "No InterOp files found" << std::endl;
            return EMPTY_INTEROP;
        }


        model::plot::filter_options options(run.run_info().flowcell().naming_method(),
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_CHANNELS,
                                            (constants::dna_bases)model::plot::filter_options::ALL_BASES,
                                            model::plot::filter_options::ALL_IDS,
                                            model::plot::filter_options::ALL_IDS
                                            //,1
                                            );
        model::plot::plot_data<model::plot::bar_point> data;
        logic::plot::plot_qscore_histogram(run, options, data);
        if(data.size() == 0 ) continue;
        std::ostream& out = std::cout;
        out << "set terminal png nocrop" << std::endl;
        out << "set output \'q-hist.png\'" << std::endl;
        out << "set style fill solid noborder" << std::endl;
        out << "set title \"" << data.title() << "\"" << std::endl;
        out << "set style data histograms" << std::endl;
        out << "set yrange [" << data.y_axis().min() << " : " << data.y_axis().max() << " ]" << std::endl;
        out << "set xrange [" << data.x_axis().min() << " : " << data.x_axis().max() << " ]" << std::endl;
        out << "set ylabel \"" << data.y_axis().label() << "\"" << std::endl;
        out << "set xlabel \"" << data.x_axis().label() << "\"" << std::endl;
        out << "plot \"-\" using 1:2:3:xtic(1) with boxes notitle" << std::endl;
        const model::plot::data_point_collection<model::plot::bar_point>& points = data[0];
        for(size_t i=0;i<points.size();++i)
        {
            out << points[i].x() << "\t" << points[i].y() << "\t" << points[i].width() << std::endl;
        }

    }
    return SUCCESS;
}


