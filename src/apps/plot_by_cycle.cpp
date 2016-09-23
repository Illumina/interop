/** @page plot_by_cycle Plot by cycle
 *
 * This application writes out a file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_by_cycle 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-147-gb6d5c19-dirty
 *      # Run Folder: 1177190_11618Unbin2R1I
 *      set terminal png nocrop
 *      set output 'plot_by_cycle.png'
 *      set title "000000000-A1W6M All Lanes Channel A All Surfaces"
 *      set yrange [0 : 850.85 ]
 *      set ylabel "Intensity"
 *      set xrange [0 : 80 ]
 *      set xlabel "Cycle"
 *      plot "-" using 1:3:2:6:5 with candlesticks notitle lt rgb "black" whiskerbars
 *      1	98.5	106	109	111	118.5
 *      2	83.5	103	110	116	135.5
 *      3	87	102	107	112	127
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
 *
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/io/plot/gnuplot.h"
#include "interop/version.h"
#include "inc/application.h"
#include "inc/plot_options.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

int test_all_filter_options(run_metrics& run);

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
    std::string metric_name="Intensity";
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
    try
    {
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

        /*if(1 == 1)
        {
            ret = test_all_filter_options(run);
            if(ret != SUCCESS) return ret;
            continue;
        }*/

        options.tile_naming_method(run.run_info().flowcell().naming_method());

        model::plot::plot_data<model::plot::candle_stick_point> data;

        try{
            logic::plot::plot_by_cycle(run, metric_name, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.size() == 0 ) continue;


        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        try
        {
            plot_writer.write_chart(out, data, plot_image_name(metric_name+"-by-cycle", run_name, metric_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

    }
    return SUCCESS;
}

void regression_test(const model::plot::plot_data<model::plot::candle_stick_point>& data)
{
    std::ifstream fin("baseline.plot");
    if(!fin.good())
    {
        std::cout << "Writing baseline" << std::endl;
        fin.close();
        std::ofstream fout("baseline.plot");
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_chart(fout, data, "plot_by_cycle.png");
    }
    else
    {
        std::cout << "Comparing to baseline" << std::endl;
        std::stringstream baseline_stream;
        baseline_stream << fin.rdbuf();

        std::ostringstream current_stream;
        io::plot::gnuplot_writer plot_writer;
        plot_writer.write_chart(current_stream, data, "plot_by_cycle.png");

        if(baseline_stream.str() != current_stream.str())
        {
            std::cout << "Baseline changed" << std::endl;
            std::istringstream baseline(baseline_stream.str());
            std::istringstream current(current_stream.str());
            for(std::string baseline_str, current_str;std::getline(baseline, baseline_str) && std::getline(current, current_str);)
            {
                if(baseline_str != current_str)
                {
                    std::cout << "Mismatch: " << baseline_str << " != " << current_str << std::endl;
                }
            }
        }
    }
}

int test_all_filter_options(run_metrics& run)
{
    const char* metric_names[] = {"Intensity", "FWHM", "CorrectedIntensity", "CalledIntensity", "BasePercent", "SignalToNoise", "ErrorRate", "Q20Percent", "Q30Percent", "QScore"};
    typedef model::plot::filter_options::id_t id_t;
    typedef model::plot::filter_options::channel_t channel_t;
    typedef model::plot::filter_options::dna_base_t dna_base_t;
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
                                                                logic::utils::is_channel_metric(metric_type) ? static_cast<channel_t>(channel) : static_cast<channel_t>(model::plot::filter_options::ALL_CHANNELS),
                                                                logic::utils::is_base_metric(metric_type) ? static_cast<dna_base_t>(base) : static_cast<dna_base_t>(model::plot::filter_options::ALL_BASES),
                                                                static_cast<id_t>(surface),
                                                                logic::utils::is_read_metric(metric_type) ? static_cast<id_t>(read) : static_cast<id_t>(ALL_IDS),
                                                                logic::utils::is_cycle_metric(metric_type) ? static_cast<id_t>(cycle) : static_cast<id_t>(ALL_IDS));

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

