/** @page plot_flowcell Plot a flowcell heatmap
 *
 * This application writes out of file that is compatible with both TSV (tab separated values) and GNUPlot, a
 * command line plotting tool available on Linux, Mac OSX and Windows.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ plot_flowcell 140131_1287_0851_A01n401drr
 *
 * In this sample, 140131_1287_0851_A01n401drr is a run folder and the summary is written to the standard output.
 *
 *      # Version: v1.0.4-147-gb6d5c19-dirty
 *      # Run Folder: 1177190_11618Unbin2R1I
 *      set terminal png nocrop
 *      set output 'flowcell.png'
 *      set title "000000000-A1W6M Intensity"
 *      set view map
 *      unset key
 *      unset tics
 *      unset border
 *      set cbrange [103:116]
 *      set palette defined (0 "blue", 0.33 "green", 0.66 "yellow", 1 "orange")
 *      plot "-" matrix with image
 *      104 110
 *      106 105
 *      105 104
 *      108 103
 *      111 106
 *      115 108
 *      109 106
 *      113 108
 *      116 108
 *      116 107
 *      114 109
 *      111 110
 *      116 109
 *      116 111
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
 */

#include <iostream>
#include <iomanip>
#include "interop/logic/utils/enums.h"
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_flowcell_map.h"
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
    options.cycle(1);
    options.dna_base(constants::A);
    options.channel(0);
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

    try{
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

        options.tile_naming_method(run.run_info().flowcell().naming_method());
        /*if( 1 == 0)
        {
            ret = test_all_filter_options(run);
            if(ret != SUCCESS) return ret;
            continue;
        }*/

        model::plot::flowcell_data data;
        try
        {
            logic::plot::plot_flowcell_map(run, metric_name, options, data);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }

        if(data.length() == 0 ) continue;
        std::ostream& out = std::cout;
        io::plot::gnuplot_writer plot_writer;
        try{
            plot_writer.write_flowcell(out, data, plot_image_name("flowcell-"+metric_name, run_name));
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        //plot_writer.write_flowcell_tile_id(out, data, plot_image_name("flowcell-"+metric_name, run_name));
    }
    return SUCCESS;
}


int test_all_filter_options(run_metrics& run)
{
    const char* metric_names[] = {"Intensity", "FWHM", "CorrectedIntensity", "CalledIntensity", "BasePercent",
                                  "SignalToNoise", "ErrorRate", "Q20Percent", "Q30Percent", "QScore", "Clusters",
                                  "ClustersPF", "ClusterCount",
                                  "ClusterCountPF", "PercentPhasing", "PercentPrephasing", "PercentAligned"};
    typedef model::plot::filter_options::id_t id_t;
    typedef model::plot::filter_options::channel_t channel_t;
    typedef model::plot::filter_options::dna_base_t dna_base_t;
    const constants::tile_naming_method naming_method = run.run_info().flowcell().naming_method();
    const size_t surface_count = run.run_info().flowcell().surface_count();
    const id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const id_t lane=ALL_IDS;
    size_t plot_count = 0;
    for(size_t i=0;i<util::length_of(metric_names);++i)
    {
        const constants::metric_type metric_type = constants::parse<constants::metric_type>(metric_names[i]);
        const size_t base_count = logic::utils::is_base_metric(metric_type) ? constants::NUM_OF_BASES : 1;
        const size_t channel_count = logic::utils::is_channel_metric(metric_type) ? run.run_info().channels().size() : 1;
        const size_t cycle_count = logic::utils::is_cycle_metric(metric_type) ? run.run_info().total_cycles() : 1;
        const size_t read_count = logic::utils::is_read_metric(metric_type) ? run.run_info().reads().size() : 1;
        for(size_t base=0;base < base_count;++base)
        {
            for(size_t channel=0;channel < channel_count;++channel)
            {
                for(size_t cycle=1;cycle <= cycle_count;++cycle)
                {
                    for(size_t read=1;read <= read_count;++read)
                    {
                        for(size_t surface=0;surface <= surface_count;++surface)
                        {
                            model::plot::filter_options options(naming_method,
                                                                lane,
                                                                logic::utils::is_channel_metric(metric_type) ? static_cast<channel_t>(channel) : static_cast<channel_t>(model::plot::filter_options::ALL_CHANNELS),
                                                                logic::utils::is_base_metric(metric_type) ? static_cast<dna_base_t>(base) : static_cast<dna_base_t>(model::plot::filter_options::ALL_BASES),
                                                                static_cast<id_t>(surface),
                                                                logic::utils::is_read_metric(metric_type) ? static_cast<id_t>(read) : ALL_IDS,
                                                                logic::utils::is_cycle_metric(metric_type) ? static_cast<id_t>(cycle) : ALL_IDS);
                            model::plot::flowcell_data data;
                            try
                            {
                                logic::plot::plot_flowcell_map(run, metric_names[i], options, data);
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

