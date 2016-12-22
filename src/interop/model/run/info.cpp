/** Parse a run into XML file
 *
 *  @file
 *  @date 11/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290 4244)  // 4290: MSVC warns that it ignores the exception specification.
// 4244: MSVC warns about  conversion from 'std::streamoff' to 'size_t', possible loss of data
// Ideally, we would not disable 4244, but the problem is in rapidxml
#endif

#include <cctype>
#include "interop/logic/utils/channel.h" // todo remove thiss
#include "interop/model/run/info.h"
#include "interop/util/xml_parser.h"
#include "interop/io/metric_stream.h"
#include "interop/logic/utils/enums.h"
#include "interop/logic/metric/tile_metric.h"


using namespace illumina::interop::xml;

namespace illumina { namespace interop { namespace model { namespace run
{

    void info::parse(char *data)  throw(xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {
        rapidxml::xml_document<> doc;
        try
        {
            doc.parse<0>(data);
        }
        catch (const rapidxml::parse_error &ex)
        {
            INTEROP_THROW(xml_parse_exception, ex.what());
        }

        xml_node_ptr p_root = doc.first_node();
        if (p_root == 0) INTEROP_THROW(empty_xml_format_exception, "Root node not found");
        if (p_root->name() != std::string("RunInfo"))
            INTEROP_THROW(bad_xml_format_exception, "Invalid run info xml file");

        for (xml_attr_ptr attr = p_root->first_attribute();
             attr; attr = attr->next_attribute())
        {
            if(set_data(attr, "Version", m_version))break;
        }


        xml_node_ptr p_run_node = p_root->first_node();

        for (; p_run_node; p_run_node = p_run_node->next_sibling())
        {
            if(p_run_node->name() == std::string("Run"))break;
        }
        if (p_run_node == 0) INTEROP_THROW(bad_xml_format_exception, "Run node not found");
        if (p_run_node->name() != std::string("Run"))
            INTEROP_THROW(bad_xml_format_exception, "Invalid run info xml file: expected Run, got: " << p_run_node->name());

        // Parse run attributes
        for (xml_attr_ptr attr = p_run_node->first_attribute();
             attr; attr = attr->next_attribute())
        {
            if(set_data(attr, "Id", m_name)) break;
        }

        // Parse run data
        for (xml_node_ptr p_node = p_run_node->first_node(); p_node; p_node = p_node->next_sibling())
        {
            if (set_data(p_node, "Date", m_date)) continue;
            if (set_data(p_node, "Flowcell", m_flowcell.m_barcode)) continue;
            if (p_node->name() == std::string("FlowcellLayout"))
            {
                for (xml_attr_ptr attr = p_node->first_attribute();
                     attr; attr = attr->next_attribute())
                {
                    if (set_data(attr, "LaneCount", m_flowcell.m_lane_count)) continue;
                    if (set_data(attr, "SurfaceCount", m_flowcell.m_surface_count)) continue;
                    if (set_data(attr, "SwathCount", m_flowcell.m_swath_count)) continue;
                    if (set_data(attr, "TileCount", m_flowcell.m_tile_count)) continue;
                    if (set_data(attr, "SectionPerLane", m_flowcell.m_sections_per_lane)) continue;
                    if (set_data(attr, "LanePerSection", m_flowcell.m_lanes_per_section)) continue;
                }
                xml_node_ptr p_tile_set = p_node->first_node("TileSet");
                if (p_tile_set == 0)
                {
                    m_flowcell.m_naming_method = constants::UnknownTileNamingMethod;
                }
                else
                {
                    std::string naming_convention;
                    set_data(p_tile_set->first_attribute("TileNamingConvention"), naming_convention);
                    m_flowcell.m_naming_method = constants::parse<constants::tile_naming_method>(naming_convention);
                    set_data(p_tile_set->first_node("Tiles"), "Tiles", "Tile", m_flowcell.m_tiles);
                }
            }
            else if (set_data(p_node, "ImageChannels", "Name", m_channels))
            {
                for (size_t i = 0; i < m_channels.size(); ++i)
                    m_channels[i] = logic::utils::normalize(m_channels[i]);// TODO: remove this
                continue;
            }
            else if (p_node->name() == std::string("ImageDimensions"))
            {
                for (xml_attr_ptr attr = p_node->first_attribute();
                     attr; attr = attr->next_attribute())
                {
                    set_data(attr, "Width", m_image_dim.m_width);
                    set_data(attr, "Height", m_image_dim.m_height);
                }
            }
            else if (p_node->name() == std::string("Reads"))
            {
                m_reads.clear();
                size_t first_cycle = 0;
                for (xml_node_ptr p_read = p_node->first_node(); p_read; p_read = p_read->next_sibling())
                {
                    read_info rinfo;
                    size_t cycle_count = 0;
                    char is_indexed;
                    for (xml_attr_ptr attr = p_read->first_attribute();
                         attr; attr = attr->next_attribute())
                    {
                        set_data(attr, "Number", rinfo.m_number);
                        if (set_data(attr, "NumCycles", cycle_count))
                        {
                            rinfo.m_last_cycle = first_cycle + cycle_count;
                            rinfo.m_first_cycle = first_cycle + 1;
                        }
                        if (set_data(attr, "IsIndexedRead", is_indexed))
                            rinfo.m_is_index = std::toupper(is_indexed) == 'Y';
                    }
                    first_cycle += cycle_count;
                    m_reads.push_back(rinfo);
                }
            }
        }
        m_total_cycle_count = total_cycles();
    }

    void info::read_file(const std::string &filename)  throw(xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {
        try
        {
            rapidxml::file<> xml_file(filename.c_str());
            parse(xml_file.data());
        } catch (const std::runtime_error &ex)
        {
            INTEROP_THROW(xml_file_not_found_exception, ex.what());
        }
    }

    void info::read(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {
        if (run_folder.find(io::paths::run_info()) != std::string::npos)
        {
            read_file(run_folder);
            return;
        }
        read_file(io::paths::run_info(run_folder));
    }
    /** Test if tile list matches flowcell layout
     *
     * @param lane lane number
     * @param tile tile number
     * @param read read number
     * @throws invalid_run_info_exception
     */
    void info::validate_read(const ::uint32_t lane, const ::uint32_t tile, const size_t read)const
    throw(invalid_run_info_exception)
    {
        validate(lane, tile);
        if(read > m_reads.size())
            INTEROP_THROW(invalid_run_info_exception, "Read number exceeds number of reads");
    }
    /** Test if tile list matches flowcell layout
     *
     * @param lane lane number
     * @param tile tile number
     * @param cycle cycle number
     * @throws invalid_run_info_exception
     */
    void info::validate_cycle(const ::uint32_t lane, const ::uint32_t tile, const size_t cycle)const
    throw(invalid_run_info_exception)
    {
        validate(lane, tile);
        if(cycle > m_total_cycle_count)
            INTEROP_THROW(invalid_run_info_exception, "Cycle number exceeds number of cycles: "
                    << cycle << " > " << m_total_cycle_count);
    }

    /** Test if tile list matches flowcell layout
     *
     * @param lane lane number
     * @param tile tile number
     * @throws invalid_run_info_exception
     */
    void info::validate(const ::uint32_t lane, const ::uint32_t tile)const throw(invalid_run_info_exception)
    {
        if(lane > m_flowcell.lane_count())
            INTEROP_THROW(invalid_run_info_exception,"Lane identifier exceeds number of lanes");
        const ::uint32_t swath = logic::metric::swath(tile, m_flowcell.naming_method());
        if(swath > m_flowcell.swath_count())
            INTEROP_THROW(invalid_run_info_exception, "Swath number exceeds number of swaths");
        const ::uint32_t tile_number = logic::metric::number(tile, m_flowcell.naming_method());
        if(tile_number > m_flowcell.tile_count())
            INTEROP_THROW(invalid_run_info_exception, "Tile number exceeds number of tiles");
        const ::uint32_t surface = logic::metric::surface(tile, m_flowcell.naming_method());
        if(surface > m_flowcell.surface_count())
            INTEROP_THROW(invalid_run_info_exception, "Surface number exceeds number of surfaces");
        const ::uint32_t section = logic::metric::section(tile, m_flowcell.naming_method());
        if(m_flowcell.m_naming_method == constants::FiveDigit && section > m_flowcell.total_number_of_sections())
            INTEROP_THROW(invalid_run_info_exception, "Section number exceeds number of sections");
    }

    /** Test if tile list matches flowcell layout
     *
     * @throws invalid_run_info_exception
     */
    void info::validate()const throw(invalid_run_info_exception, invalid_tile_naming_method)
    {
        typedef flowcell_layout::str_vector_t str_vector_t;
        if(m_flowcell.naming_method()==constants::UnknownTileNamingMethod)
            INTEROP_THROW(invalid_tile_naming_method, "Unknown tile naming method");
        std::set<size_t> unique_numbers;
        for(read_vector_t::const_iterator it = m_reads.begin();it != m_reads.end();++it)
        {
            if(unique_numbers.find(it->number()) != unique_numbers.end())
                INTEROP_THROW(invalid_run_info_exception, "Repeated read number");
            if (it->number() > m_reads.size())
                INTEROP_THROW(invalid_run_info_exception, "Missing reads");
            unique_numbers.insert(it->number());
        }
        for(str_vector_t::const_iterator it = m_flowcell.tiles().begin();it != m_flowcell.tiles().end();++it)
        {
            const ::uint32_t lane = logic::metric::lane_from_name(*it);
            if(lane == 0) INTEROP_THROW( invalid_run_info_exception, "Invalid tile identifier in tile names");
            const ::uint32_t tile = logic::metric::tile_from_name(*it);
            if(tile == 0) INTEROP_THROW( invalid_run_info_exception, "Invalid tile identifier in tile names");
            validate(lane, tile);
        }
    }

}}}}


#ifdef _MSC_VER
#pragma warning(pop)
#endif


