/** Option parsing code fo shared plot options
 *
 * This is a private header file outside of any namespaces, beware!
 *
 *  @file
 *  @date 6/30/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/plot/filter_options.h"

namespace illumina { namespace interop { namespace util
{
    // todo move to logic?
    inline std::istream &operator>>(std::istream &in, constants::dna_bases &bases)
    {
        std::string value;
        std::getline(in, value);
        constants::dna_bases val = constants::parse<constants::dna_bases>(value);
        if (val == constants::UnknownBase)
        {
            std::istringstream iss(value);
            int tmp = static_cast<int>(constants::UnknownBase);
            iss >> tmp;
            if (iss.eof())
                bases = static_cast<constants::dna_bases>(tmp);
            else bases = constants::UnknownBase;
        }
        else bases = val;
        return in;
    }
}}}

#include "interop/util/option_parser.h"

/** Add option to parse metric name
 *
 * This adds the following options to the parser:
 *   - `--metric-name=<name>`: Metric to plot
 *
 * @param description option parser
 * @param metric_name value to hold metric name
 */
void add_metric_option(illumina::interop::util::option_parser& description, std::string& metric_name)
{
    description
            (metric_name, "metric-name", "Metric to plot");
}
/** Add options to parse filter options
 *
 * This adds the following options to the parser:
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
 * @param description option parser
 * @param options value to hold filter options
 */
void add_filter_options(illumina::interop::util::option_parser& description,
                        illumina::interop::model::plot::filter_options& options)
{
    using namespace illumina::interop;
    std::string group = "filter-by-";
    description
            (util::wrap_setter(options, &model::plot::filter_options::lane), group+"lane", "Only the data for the selected lane will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::channel), group+"channel", "Only the data for the selected channel will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::dna_base), group+"base", "Only the data for the selected base will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::surface), group+"surface", "Only the data for the selected surface will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::read), group+"read", "Only the data for the selected read will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::cycle), group+"cycle", "Only the data for the selected cycle will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::tile_number), group+"tile-number", "Only the data for the selected tile number will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::swath), group+"swath", "Only the data for the selected swath will be displayed")
            (util::wrap_setter(options, &model::plot::filter_options::section), group+"section", "Only the data for the selected section will be displayed");
}

/** Create a default image file name
 *
 * @param plot_name name of the plot
 * @param run_name name of the run folder
 * @param metric_name optional metric name
 * @param extra extra info
 * @return plot image filename
 */
std::string plot_image_name(const std::string& plot_name,
                            const std::string& run_name,
                            const std::string& metric_name="",
                            const std::string& extra="")
{
    std::string name = run_name+"_"+plot_name;
    if(metric_name != "") name += "_"+metric_name;
    if(extra != "") name += "_"+extra;
    return name+".png";
}

