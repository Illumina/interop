/** Additional metadata describing a sequencing run
 *
 * The InterOp files parsed by this class include:
 *  - RunParameters.xml
 *
 *  @file
 *  @date 11/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <string>
#include <vector>
#include "interop/util/exception.h"
#include "interop/constants/enums.h"
#include "interop/util/xml_exceptions.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290)// MSVC warns that it ignores the exception specification.
#endif

namespace illumina { namespace interop { namespace model { namespace run
{

    /** Metadata describing sequencing run
     *
     * This class parses the RunParameters.xml XML file. It only provides
     * the instrument type and version of the XML file format. We do not plan
     * to support parsing any other values in this file.
     *
     * This class is only used on legacy platforms where:
     *  1. The channel names were not given in the RunInfo.xml
     *  2. The bins were not provided in the header of the QMetrics.bin (v4)
     *
     *  We do not support parsing this file on later platforms.
     */
    class parameters
    {
    public:
        /** Unsigned integer type */
        typedef ::uint32_t uint_t;
        /** Instrument type enum */
        typedef constants::instrument_type instrument_type_t;

    public:
        /** Constructor
         *
         * @param version XML format version
         * @param instrument_type type code of the instrument
         */
        parameters(const uint_t version = 0,
                   const instrument_type_t instrument_type = constants::UnknownInstrument
        ) :
                m_version(version),
                m_instrument_type(instrument_type)
        {
        }

    public:
        /** Get the type of instrument
         *
         * @return type of instrument
         */
        instrument_type_t instrument_type() const
        { return m_instrument_type; }

        /** Get the version of the xml format
         *
         * @return version of the xml format
         */
        uint_t version() const
        { return m_version; }

    public:
        /** Read run metadata from the given run folder
         *
         * @param run_folder run folder containing RunParameters.xml
         */
        void read(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** Read run metadata from the given XML file
         *
         * @param filename xml file
         */
        void read_file(const std::string &filename) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** String containing xml data
         *
         * @param data xml string data
         */
        void parse(char *data) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

    private:
        void set_instrument_id(std::string &application_name, std::string &multi_surface);

    private:
        uint_t m_version;
        instrument_type_t m_instrument_type;
    };
}}}}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

