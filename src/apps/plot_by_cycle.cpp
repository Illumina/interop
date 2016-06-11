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

int test_all_filter_options(run_metrics& run);

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

        if(1 == 0)
        {
            ret = test_all_filter_options(run);
            if(ret != SUCCESS) return ret;
            continue;
        }

        model::plot::filter_options options(run.run_info().flowcell().naming_method());
        options.surface(constants::Top);

        model::plot::plot_data<model::plot::candle_stick_point> data;

        try{
            logic::plot::plot_by_cycle(run, constants::FWHM, options, data);
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


int test_all_filter_options(run_metrics& run)
{
    const char* metric_names[] = {"Intensity", "FWHM", "CorrectedIntensity", "CalledIntensity", "BasePercent", "SignalToNoise", "ErrorRate", "Q20Percent", "Q30Percent", "QScore"};
    typedef model::plot::filter_options::id_t id_t;
    const constants::tile_naming_method naming_method = run.run_info().flowcell().naming_method();
    const size_t surface_count = run.run_info().flowcell().surface_count();
    const id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const id_t lane=ALL_IDS;
    const id_t cycle = ALL_IDS;
    const id_t read = ALL_IDS;
    size_t plot_count = 0;
    for(size_t i=0;i<util::length_of(metric_names);++i)
    {
        const constants::metric_type metric_type = constants::parse<constants::metric_type>(metric_names[i]);
        const size_t base_count = logic::utils::is_base_metric(metric_type) ? constants::NUM_OF_BASES : 1;
        const size_t channel_count = logic::utils::is_channel_metric(metric_type) ? run.run_info().channels().size() : 1;
        //const size_t cycle_count = logic::utils::is_cycle_metric(metric_type) ? run.run_info().total_cycles() : 1;
        //const size_t read_count = logic::utils::is_read_metric(metric_type) ? run.run_info().reads().size() : 1;
        for(size_t base=0;base < base_count;++base)
        {
            for(size_t channel=0;channel < channel_count;++channel)
            {
                //for(size_t cycle=1;cycle <= cycle_count;++cycle)
                {
                    //for(size_t read=1;read <= read_count;++read)
                    {
                        for(size_t surface=0;surface <= surface_count;++surface)
                        {
                            model::plot::filter_options options(naming_method,
                                                                lane,
                                                                logic::utils::is_channel_metric(metric_type) ? (id_t)channel : (id_t)model::plot::filter_options::ALL_CHANNELS,
                                                                (constants::dna_bases) (logic::utils::is_base_metric(metric_type) ? base : (size_t)model::plot::filter_options::ALL_BASES),
                                                                surface,
                                                                logic::utils::is_read_metric(metric_type) ? read : ALL_IDS,
                                                                logic::utils::is_cycle_metric(metric_type) ? cycle : ALL_IDS);

                            model::plot::plot_data<model::plot::candle_stick_point> data;

                            try{
                                logic::plot::plot_by_cycle(run, metric_names[i], options, data);
                            }
                            catch(const std::exception& ex)
                            {
                                std::cerr << ex.what() << std::endl;
                                return UNEXPECTED_EXCEPTION;
                            }
                            plot_count++;
                        }
                    }
                }
            }
        }
    }
    std::cout << "I just created " << plot_count << " plots" << std::endl;
    return SUCCESS;
}
