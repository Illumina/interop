/** Parse a run parameters XML file
 *
 *  @file
 *  @date 11/9/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290 4244)  // 4290: MSVC warns that it ignores the exception specification.
                                    // 4244: MSVC warns about  conversion from 'std::streamoff' to 'size_t', possible loss of data
                                    // Ideally, we would not disable 4244, but the problem is in rapidxml
#endif

#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <ctime>
#include "interop/model/run/parameters.h"
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

void parameters::parse(char *data)  _INTEROP_XML_THROWS
{
    rapidxml::xml_document<> doc;
    try{
        doc.parse<0>(data);
    }
    catch(const rapidxml::parse_error& ex)
    {
        throw xml_parse_exception(ex.what());
    }

    m_version=0;
    xml_node_ptr p_root = doc.first_node();
    if(p_root == 0) throw empty_xml_format_exception("Root not found");
    if( p_root->name() != std::string("RunParameters")) throw bad_xml_format_exception("Invalid run parameters xml file");
    set_data_with_default(p_root->first_node("Version"), "Version", m_version, 0u);

    // Parse run data
    std::string application_name;
    std::string multi_surface;
    m_instrument_type = constants::UnknownInstrument;

    xml_node_ptr p_setup = p_root->first_node("Setup");
    if(p_setup == 0) throw bad_xml_format_exception("Setup not found");
    for(xml_node_ptr p_node = p_setup->first_node();p_node;p_node = p_node->next_sibling())
    {
        set_data(p_node, "ApplicationName", application_name);
        set_data(p_node, "SupportMultipleSurfacesInUI", multi_surface);
    }
    set_instrument_id(application_name, multi_surface);

}


void parameters::set_instrument_id(std::string& application_name, std::string& multi_surface)
{
    std::transform(application_name.begin(), application_name.end(), application_name.begin(), ::tolower);
    std::transform(multi_surface.begin(), multi_surface.end(), multi_surface.begin(), ::tolower);
    if(application_name.find("hiseq") != std::string::npos)
    {
        m_instrument_type = constants::HiSeq;
    }
    else if(application_name.find("miseq") != std::string::npos)
    {
        m_instrument_type = constants::MiSeq;
    }
    else if(application_name.find("nextseq") != std::string::npos)
    {
        m_instrument_type = constants::NextSeq;
    }
    else if(application_name.find("miniseq") != std::string::npos)
    {
        m_instrument_type = constants::MiniSeq;
    }
    else
    {
        m_instrument_type = constants::UnknownInstrument;
    }
    if(multi_surface != "" && m_instrument_type == constants::HiSeq)
    {
        // TODO: check if this even works! Find HiScan Example
        if(multi_surface == "0" || multi_surface == "false" || multi_surface == "f")
            m_instrument_type = constants::HiScan;
    }
}

void parameters::read_file(const std::string &filename)  _INTEROP_XML_THROWS
{
    try
    {
        rapidxml::file<> xmlFile(filename.c_str());
        parse(xmlFile.data());
    }catch(const std::runtime_error& ex)
    {
        throw xml_file_not_found_exception(ex.what());
    }
}

void parameters::read(const std::string& run_folder) _INTEROP_XML_THROWS
{
    read_file(io::combine(run_folder, "RunParameters.xml"));
}


}
}
}
}


#ifdef _MSC_VER
#pragma warning(pop)
#endif

