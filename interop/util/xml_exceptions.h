/** Exceptions thrown by XML parsing
 *
 *  @file
 *  @date 11/11/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <stdexcept>

namespace illumina { namespace interop { namespace xml
{

    /** Base exception for all format errors
     */
    struct xml_format_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        xml_format_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };
    /** @defgroup xml_exceptions XML Parsing Exceptions
     *
     * Exceptions that will be thrown if a problem occurs while reading an XML file.
     *
     * @ingroup interop_exceptions
     * @{
     */
    /** Exception raised if the XML file is not found in the file system
     */
    struct xml_file_not_found_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        xml_file_not_found_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the XML does not have the correct format
     *
     * This is only raised if the format is bad.
     */
    struct xml_parse_exception : public xml_format_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        xml_parse_exception(const std::string &mesg) : xml_format_exception(mesg)
        { }
    };

    /** Exception raised if the XML does not have the correct format
     *
     * This is only raised if the format is bad.
     */
    struct bad_xml_format_exception : public xml_format_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        bad_xml_format_exception(const std::string &mesg) : xml_format_exception(mesg)
        { }
    };

    /** Exception raised if the XML file record is empty
     */
    struct empty_xml_format_exception : public xml_format_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        empty_xml_format_exception(const std::string &mesg) : xml_format_exception(mesg)
        { }
    };

    /** Exception raised if the XML file is missing a required element
     */
    struct missing_xml_element_exception : public xml_format_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        missing_xml_element_exception(const std::string &mesg) : xml_format_exception(mesg)
        { }
    };

    /** @} */

}}}

