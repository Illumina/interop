/** Factory for generating text formats
 *
 *  @file
 *  @date 12/19/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/map.h"
#include "interop/util/assert.h"
#include "interop/util/self_registration.h"
#include "interop/io/format/abstract_text_format.h"
#include "interop/util/unique_ptr.h"
#include "interop/io/stream_exceptions.h"

/** Register a text format with the factory
 *
 * @param Metric metric class
 * @param Version version number
 */
#define INTEROP_REGISTER_METRIC_TEXT_LAYOUT(Metric, Version) \
    illumina::interop::io::text_format_factory_proxy< Metric >  \
    illumina_interop_io_text_##Type##Metric##Version(new illumina::interop::io::text_format<Metric, illumina::interop::io::text_layout<Metric, Version> >);


namespace illumina { namespace interop { namespace io
{
    /** Factory for generating text formats
     *
     * This class defines static methods to register a metric format. The registered metric formats can
     * be accessed through the `instance()` static function.
     *
     * @note this is not thread safe
     */
    template<class Metric>
    struct text_format_factory
    {
        /** Define the metric type */
        typedef Metric metric_type;
        /** Define the abstract format type */
        typedef abstract_text_format<metric_type> abstract_text_format_t;
        /** Define the header type */
        typedef typename Metric::header_type header_type;
        /** Define a unique pointer to a metric format */
        typedef stdbp::unique_ptr<abstract_text_format_t> metric_format_pointer;
        /** Define a map between format version and the format */
        typedef INTEROP_UNORDERED_MAP(int, metric_format_pointer) text_format_map;

        /** Find a format for a given version
         *
         * If the format is not found, return null
         *
         * @param version version to search for
         * @return pointer to format or null
         */
        abstract_text_format_t* find(int version)
        {
            if(version < 0) version = m_latest_version;
            typename text_format_map::iterator it = m_format_map.find(version);
            if(it == m_format_map.end()) return 0;
            return &(*it->second);
        }

        /** Add a text format to the factory
         *
         * @param pformat format to add
         */
        void add(abstract_text_format_t *pformat)
        {
            const int version = pformat->version();
            if(version > m_latest_version) m_latest_version = version;
            m_format_map[version] = metric_format_pointer(pformat);
        }
        /** Get number of text formats
         *
         * @return number of text formats
         */
        size_t size()const
        {
            return m_format_map.size();
        }

        /** Instance of the factory singleton
         *
         * @note this is not thread safe
         * @return instance to singleton
         */
        static text_format_factory<Metric> &instance()
        {
            INTEROP_FORCE_LINK_USE(metric_type);
            static text_format_factory<Metric> _inst;
            return _inst;
        }

    private:
        text_format_factory() : m_latest_version(-1) {}
        text_format_map m_format_map;
        int m_latest_version;

    };
    /** Proxy for registering text formats
     */
    template<class Metric>
    struct text_format_factory_proxy
    {
        /** Define the metric type */
        typedef Metric metric_type;
        /** Define the abstract format type */
        typedef abstract_text_format<metric_type> abstract_text_format_t;
        /** Constructor
         *
         * This constructor is used to statically register a text format in a source file.
         */
        text_format_factory_proxy(abstract_text_format_t *pformat)
        {
            text_format_factory<Metric>::instance().add(pformat);
        }
    };

}}}



