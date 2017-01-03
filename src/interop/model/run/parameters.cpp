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
#include "interop/util/xml_parser.h"
#include "interop/io/metric_stream.h"
#include "interop/logic/utils/enums.h"


using namespace illumina::interop::xml;

namespace illumina { namespace interop { namespace model { namespace run
{

    void parameters::parse(char *data) throw(xml::xml_file_not_found_exception,
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

        m_version = 0;
        xml_node_ptr p_root = doc.first_node();
        if (p_root == 0) INTEROP_THROW(empty_xml_format_exception, "Root not found");
        if (p_root->name() != std::string("RunParameters"))
            INTEROP_THROW(bad_xml_format_exception, "Invalid run parameters xml file");
        set_data_with_default(p_root->first_node("Version"), "Version", m_version, 0u);

        // Parse run data
        std::string application_name;
        std::string multi_surface;
        m_instrument_type = constants::UnknownInstrument;

        xml_node_ptr p_setup = p_root->first_node("Setup");
        if (p_setup == 0) INTEROP_THROW(bad_xml_format_exception, "Setup not found");
        for (xml_node_ptr p_node = p_setup->first_node(); p_node; p_node = p_node->next_sibling())
        {
            set_data(p_node, "ApplicationName", application_name);
            set_data(p_node, "SupportMultipleSurfacesInUI", multi_surface);
        }
        set_instrument_id(application_name, multi_surface);

    }


    void parameters::set_instrument_id(std::string &application_name, std::string &multi_surface)
    {
        std::transform(application_name.begin(), application_name.end(), application_name.begin(), ::tolower);
        std::transform(multi_surface.begin(), multi_surface.end(), multi_surface.begin(), ::tolower);

        for (size_t i = 0; i < constants::InstrumentCount; ++i)
        {
            std::string instrument_name = constants::to_string(static_cast<constants::instrument_type>(i));
            std::transform(instrument_name.begin(), instrument_name.end(), instrument_name.begin(), ::tolower);
            if (application_name.find(instrument_name) != std::string::npos)
            {
                m_instrument_type = static_cast<constants::instrument_type>(i);
                break;
            }
        }
        if (multi_surface != "" && m_instrument_type == constants::HiSeq)
        {
            // TODO: check if this even works! Find HiScan Example
            if (multi_surface == "0" || multi_surface == "false" || multi_surface == "f")
                m_instrument_type = constants::HiScan;
        }
    }

    void parameters::read_file(const std::string &filename)
    throw(xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {
        try
        {
            rapidxml::file<> xml_file(filename.c_str());
            parse(xml_file.data());
        }
        catch (const std::runtime_error &ex)
        {
            INTEROP_THROW(xml_file_not_found_exception, ex.what());
        }
    }

    void parameters::read(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {

        if (run_folder.find(io::paths::run_parameters()) != std::string::npos ||
            run_folder.find(io::paths::run_parameters(true)) != std::string::npos)
        {
            read_file(run_folder);
            return;
        }
        try
        {
            read_file(io::paths::run_parameters(run_folder, true));
        }
        catch (const xml_file_not_found_exception &)
        {
            read_file(io::paths::run_parameters(run_folder));
        }

    }

}}}}


#ifdef _MSC_VER
#pragma warning(pop)
#endif


