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
#include "interop/util/exception.h"
#include "interop/util/lexical_cast.h"
#include "interop/model/run/flowcell_layout.h"
#include "interop/model/run/image_dimensions.h"
#include "interop/model/run/read_info.h"
#include "interop/util/xml_exceptions.h"
#include "interop/model/model_exceptions.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

namespace illumina { namespace interop { namespace model { namespace run
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
        typedef std::vector<std::string> str_vector_t;
        /** Read vector type */
        typedef std::vector<read_info> read_vector_t;
        /** Constant read iterator type */
        typedef read_vector_t::const_iterator const_read_iterator;
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
        info(const std::string &name = "",
             const std::string &date = "",
             const uint_t version = 0,
             const flowcell_layout &flowcell = flowcell_layout(),
             const str_vector_t &channels = str_vector_t(),
             const image_dimensions &image_dim = image_dimensions(),
             const read_vector_t &reads = read_vector_t()) :
                m_name(name),
                m_date(date),
                m_version(version),
                m_flowcell(flowcell),
                m_channels(channels),
                m_image_dim(image_dim),
                m_reads(reads),
                m_total_cycle_count(0)
        {
            m_total_cycle_count = total_cycles();
        }

        /** Constructor
         *
         * @param flowcell layout of the flowcell
         * @param channels string list of channel names
         * @param reads description of the reads
         */
        info(const flowcell_layout &flowcell,
             const str_vector_t &channels=str_vector_t(),
             const read_vector_t &reads=read_vector_t()) :
                m_version(0),
                m_flowcell(flowcell),
                m_channels(channels),
                m_reads(reads),
                m_total_cycle_count(0)
        {
            m_total_cycle_count = total_cycles();
        }

    public:
        /** @defgroup run_info Run Info
         *
         * Information describing the run
         *
         * @ingroup run_metrics
         * @ref illumina::interop::model::run::info "See full class description"
         * @{
         */

        /** Read run information from run folder
         *
         * @param run_folder run folder containing RunInfo.xml
         */
        void read(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** Test if tile list matches flowcell layout
         *
         * @throws invalid_run_info_exception
         */
        void validate()const throw(invalid_run_info_exception,invalid_tile_naming_method);
        /** Test if tile list matches flowcell layout
         *
         * @param lane lane number
         * @param tile tile number
         * @throws invalid_run_info_exception
         */
        void validate(const ::uint32_t lane, const ::uint32_t tile)const throw(invalid_run_info_exception);
        /** Test if tile list matches flowcell layout
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @throws invalid_run_info_exception
         */
        void validate_cycle(const ::uint32_t lane, const ::uint32_t tile, const size_t cycle)const
        throw(invalid_run_info_exception);
        /** Test if tile list matches flowcell layout
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         * @throws invalid_run_info_exception
         */
        void validate_read(const ::uint32_t lane, const ::uint32_t tile, const size_t read)const
        throw(invalid_run_info_exception);

    public:
        /** Get the name of the run
         *
         * @return name of the run
         */
        const std::string &name() const
        { return m_name; }

        /** Get the date of the run
         *
         * @return date of the run
         */
        const std::string &date() const
        { return m_date; }

        /** Get the version of the XML file format
         *
         * @return version of the XML file format
         */
        uint_t version() const
        { return m_version; }

        /** Get the layout of the flowcell
         *
         * @return layout of the flowcell
         */
        const flowcell_layout &flowcell() const
        { return m_flowcell; }

        /** Get the channel labels
         *
         * @return channel labels
         */
        const str_vector_t &channels() const
        { return m_channels; }

        /** Get the dimensions of the tile image
         *
         * @return tile image dimensions
         */
        const image_dimensions &dimensions_of_image() const
        { return m_image_dim; }

        /** Get the read info
         *
         * @return reads
         */
        const read_vector_t &reads() const
        { return m_reads; }

        /** Check if there is an index read
         *
         * @return true if there is an index read
         */
        bool is_indexed() const
        {
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                if (b->is_index()) return true;
            return false;
        }
        /** Test if cycle is last cycle of a read
         *
         * @param cycle cycle number to test
         * @return true if the cycle number equals the last cycle of a read
         */
        bool is_last_cycle_of_read(const size_t cycle)const
        {
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                if (b->last_cycle() == cycle) return true;
            return false;
        }
        /** Get the cycle number within a read (0 is returned for invalid cycles)
         *
         * @param cycle cycle number to test
         * @return cycle number within read
         */
        size_t cycle_within_read(const size_t cycle)const
        {
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                if ( cycle <= b->last_cycle()) return cycle-b->first_cycle() + 1;
            return 0;
        }

        /** Get read with given number
         *
         * @param read_number number of the read
         * @return read
         */
        const read_info &read(const size_t read_number) const
        {
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                if (b->number() == read_number) return *b;
            INTEROP_THROW(invalid_read_exception, "Read number not found: " << read_number);
        }

        /** Set the channel labels
         *
         * @param channels channel labels
         */
        void channels(const str_vector_t &channels)
        { m_channels = channels; }

        /** Set the reads info
         *
         * @param read_vec reads info
         */
        void reads(const read_vector_t& read_vec)
        {
            m_reads = read_vec;
            m_total_cycle_count = total_cycles();
        }

        /** Set the tile naming method
         *
         * @param naming_method tile naming method
         */
        void set_naming_method(const constants::tile_naming_method naming_method)
        {
            m_flowcell.set_naming_method(naming_method);
        }
        /** Set the layout of the flowcell
         *
         * @param flowcell layout of the flowcell
         */
        void flowcell(const flowcell_layout & flowcell)
        { m_flowcell = flowcell; }

        /** Get total number of cycles
         *
         * @return total number of cycles
         */
        size_t total_cycles() const
        {
            size_t total = 0;
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                total += b->total_cycles();
            return total;
        }

        /** Get usable number of cycles
         *
         * @return usable number of cycles
         */
        size_t useable_cycles() const
        {
            size_t total = 0;
            for (read_vector_t::const_iterator b = m_reads.begin(), e = m_reads.end(); b != e; ++b)
                total += b->useable_cycles();
            return total;
        }
        /** @} */

    public:
        /** Read run information from the given XML file
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
        std::string m_name;
        std::string m_date;
        uint_t m_version;
        flowcell_layout m_flowcell;
        str_vector_t m_channels;
        image_dimensions m_image_dim;
        read_vector_t m_reads;
        size_t m_total_cycle_count;
    };

}}}}

#ifdef _MSC_VER
#pragma warning(pop)
#endif


