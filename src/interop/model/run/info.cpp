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
#include "interop/model/run/info.h"
#include "interop/util/lexical_cast.h"
#include "interop/util/xml_parser.h"
#include "interop/io/metric_stream.h"




using namespace illumina::interop::xml;

namespace illumina
{
namespace interop
{
namespace model
{
namespace run
{

void info::parse(char *data)  throw(    xml::xml_file_not_found_exception,
                                        xml::bad_xml_format_exception,
                                        xml::empty_xml_format_exception,
                                        xml::missing_xml_element_exception,
                                        xml::xml_parse_exception)
{
    rapidxml::xml_document<> doc;
    try{
        doc.parse<0>(data);
    }
    catch(const rapidxml::parse_error& ex)
    {
        throw xml_parse_exception(ex.what());
    }

    xml_node_ptr p_root = doc.first_node();
    if(p_root == 0) throw empty_xml_format_exception("Root not found");
    if( p_root->name() != std::string("RunInfo")) throw bad_xml_format_exception("Invalid run info xml file");

    for (xml_attr_ptr attr = p_root->first_attribute();
         attr; attr = attr->next_attribute())
    {
        set_data(attr, "Version", m_version);
    }


    xml_node_ptr p_run_node = p_root->first_node();
    if(p_run_node == 0) throw bad_xml_format_exception("Run not found");
    if( p_run_node->name() != std::string("Run")) throw bad_xml_format_exception("Invalid run info xml file");

    // Parse run attributes
    for (xml_attr_ptr attr = p_run_node->first_attribute();
         attr; attr = attr->next_attribute())
    {
        set_data(attr, "Id", m_name);
    }

    // Parse run data
    for(xml_node_ptr p_node = p_run_node->first_node();p_node;p_node = p_node->next_sibling())
    {
        if(set_data(p_node, "Date", m_date)) continue;
        if(set_data(p_node, "Flowcell", m_flowcell.m_barcode)) continue;
        if(p_node->name() == std::string("FlowcellLayout"))
        {
            for (xml_attr_ptr attr = p_node->first_attribute();
              attr; attr = attr->next_attribute())
            {
                if(set_data(attr, "LaneCount", m_flowcell.m_lane_count)) continue;
                if(set_data(attr, "SurfaceCount", m_flowcell.m_surface_count)) continue;
                if(set_data(attr, "SwathCount", m_flowcell.m_swath_count)) continue;
                if(set_data(attr, "TileCount", m_flowcell.m_tile_count)) continue;
                if(set_data(attr, "SectionPerLane", m_flowcell.m_sections_per_lane)) continue;
                if(set_data(attr, "LanePerSection", m_flowcell.m_lanes_per_section)) continue;
            }
            xml_node_ptr p_tile_set = p_node->first_node("TileSet");
            if(p_tile_set == 0)
            {
                m_flowcell.m_naming_method = constants::absolute;
            }
            else
            {
                std::string naming_convention;
                set_data(p_tile_set->first_attribute("TileNamingConvention"), naming_convention);
                m_flowcell.m_naming_method = constants::tile_naming_from_string(naming_convention);
                set_data(p_tile_set->first_node("Tiles"), "Tiles", m_flowcell.m_tiles);
            }
        }
        else if(set_data(p_node, "ImageChannels", m_channels)) continue;
        else if(p_node->name() == std::string("ImageDimensions"))
        {
            for (xml_attr_ptr attr = p_node->first_attribute();
                 attr; attr = attr->next_attribute()) {
                set_data(attr, "Width", m_image_dim.m_width);
                set_data(attr, "Height", m_image_dim.m_height);
            }
        }
        else if(p_node->name() == std::string("Reads"))
        {
            m_reads.clear();
            size_t first_cycle = 0;
            for(xml_node_ptr p_read = p_node->first_node();p_read;p_read = p_read->next_sibling())
            {
                read_info rinfo;
                size_t cycle_count=0;
                char is_indexed;
                for (xml_attr_ptr attr = p_read->first_attribute();
                     attr; attr = attr->next_attribute())
                {
                    set_data(attr, "Number", rinfo.m_number);
                    set_data(attr, "NumCycles", cycle_count);
                    set_data(attr, "IsIndexedRead", is_indexed);
                    rinfo.m_first_cycle = first_cycle+1;
                    rinfo.m_last_cycle = first_cycle+cycle_count;
                    rinfo.m_is_index = std::toupper(is_indexed) == 'Y';
                }
                first_cycle+=cycle_count;
                m_reads.push_back(rinfo);
            }
        }
    }

}

void info::read_file(const std::string &filename)  throw(   xml::xml_file_not_found_exception,
                                                            xml::bad_xml_format_exception,
                                                            xml::empty_xml_format_exception,
                                                            xml::missing_xml_element_exception,
                                                            xml::xml_parse_exception)
{
    try{
        rapidxml::file<> xmlFile(filename.c_str());
        parse(xmlFile.data());
    }catch(const std::runtime_error& ex)
    {
        throw xml_file_not_found_exception(ex.what());
    }
}

void info::read(const std::string& run_folder) throw(   xml::xml_file_not_found_exception,
                                                        xml::bad_xml_format_exception,
                                                        xml::empty_xml_format_exception,
                                                        xml::missing_xml_element_exception,
                                                        xml::xml_parse_exception)
{
    read_file(io::combine(run_folder, "RunInfo.xml"));
}

}
}
}
}


#ifdef _MSC_VER
#pragma warning(pop)
#endif

