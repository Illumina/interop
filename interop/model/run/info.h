/** Information describing a sequencing run
 *
 * The InterOp files parsed by this class include:
 *  - RunInfo.xml
 *
 *  @file
 *  @date 11/7/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <vector>
#include "interop/model/run/flowcell_layout.h"
#include "interop/model/run/image_dimensions.h"
#include "interop/model/run/read_info.h"
#include "interop/util/xml_exceptions.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

namespace illumina
{
namespace interop
{
namespace model
{
namespace run
{

/** Metadata describing parameters that can change between runs
 *
 * This class parses the RunInfo.xml XML file
 */
class info
{
public:
    /** Unsigned integer type */
    typedef ::uint32_t uint_t;
    /** String vector type */
    typedef std::vector <std::string> str_vector_t;
    /** Read vector type */
    typedef std::vector< illumina::interop::model::run::read_info> read_vector_t;
public:
    /** Constructor
     *
     * @param name name of the run
     * @param date date of the run
     * @param version xml file format version
     * @param flowcell layout of the flowcell
     * @param channels names of the color channels
     * @param image_dim dimensions of the image
     * @param reads description of the reads
     */
    info(const std::string& name="",
         const std::string& date="",
         const uint_t version=0,
         const flowcell_layout& flowcell=flowcell_layout(),
         const str_vector_t& channels=str_vector_t(),
         const image_dimensions& image_dim=image_dimensions(),
         const read_vector_t& reads=read_vector_t()) :
            m_name(name),
            m_date(date),
            m_version(version),
            m_flowcell(flowcell),
            m_channels(channels),
            m_image_dim(image_dim),
            m_reads(reads)
    {
    }
    /** Constructor
     *
     * @param flowcell layout of the flowcell
     * @param channels string list of channel names
     * @param reads description of the reads
     */
    info(const flowcell_layout& flowcell,
         const str_vector_t& channels,
         const read_vector_t& reads) :
            m_version(0),
            m_flowcell(flowcell),
            m_channels(channels),
            m_reads(reads)
    {
    }

public:
    /** Read run information from run folder
     *
     * @param run_folder run folder containing RunInfo.xml
     */
    void read(const std::string& run_folder) _INTEROP_XML_THROWS;
    /** Read run information from the given XML file
     *
     * @param filename xml file
     */
    void read_file(const std::string& filename) _INTEROP_XML_THROWS;
    /** String containing xml data
     *
     * @param data xml string data
     */
    void parse(char* data) _INTEROP_XML_THROWS;

public:
    /** Get the name of the run
     *
     * @return name of the run
     */
    const std::string& name()const{return m_name;}
    /** Get the date of the run
     *
     * @return date of the run
     */
    const std::string& date()const{return m_date;}
    /** Get the version of the XML file format
     *
     * @return version of the XML file format
     */
    uint_t version()const{return m_version;}
    /** Get the layout of the flowcell
     *
     * @return layout of the flowcell
     */
    const flowcell_layout& flowcell()const{ return m_flowcell;}
    /** Get the channel labels
     *
     * @return channel labels
     */
    const str_vector_t& channels()const{ return m_channels;}
    /** Get the dimensions of the tile image
     *
     * @return tile image dimensions
     */
    const image_dimensions& dimensions_of_image()const{ return m_image_dim;}
    /** Get the read info
     *
     * @return reads
     */
    const read_vector_t & reads()const{ return m_reads;}
    /** Set the channel labels
     *
     * @param channels channel labels
     */
    void channels(const str_vector_t& channels){ m_channels = channels;}


private:
    std::string m_name;
    std::string m_date;
    uint_t m_version;
    flowcell_layout m_flowcell;
    str_vector_t m_channels;
    image_dimensions m_image_dim;
    read_vector_t m_reads;
};

}
}
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

