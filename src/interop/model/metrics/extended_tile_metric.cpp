/** Register format layouts for the extended tile metrics
 *
 * Each version of the extended tile metrics file has a layout defined below.
 *
 *  @file
 *  @date 10/22/15
 *  @version 1.0
 *  @copyright GNU Public License
 */
#include "interop/util/type_traits.h"
#include "interop/model/metrics/extended_tile_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/text_format.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io
{
#pragma pack(1)
        /** Extended Tile Metric Record Layout Version 1
         *
         * This class provides an interface to reading the extended tile metric file:
         *  - InterOp/ExtendedTileMetrics.bin
         *  - InterOp/ExtendedTileMetricsOut.bin
         *
         * The class takes two template arguments:
         *
         *      1. Metric Type: extended_tile_metric
         *      2. Version: 1
         */
        template<>
        struct generic_layout<extended_tile_metric, 1> : public default_layout<1>
        {
            /** @page extended_tile_v1 Extended Tile Version 1
             *
             * This class provides an interface to reading the extended tile metric file:
             *  - InterOp/ExtendedTileMetrics.bin
             *  - InterOp/ExtendedTileMetricsOut.bin
             *
             *  The file format for extended tile metrics is as follows:
             *
             *  @b Header
             *
             *  illumina::interop::io::metric_format_stream (Class that parses this information)
             *
             *          byte 0: version number (1)
             *          byte 1: record size (10)
             *
             *  @b n-Records
             *
             *  illumina::interop::io::layout::base_metric (Class that parses this information)
             *
             *          2 bytes: lane number (uint16)
             *          2 bytes: tile number (uint16)
             *
             *  illumina::interop::io::generic_layout<extended_tile_metric, 1> (Class that parses this information)
             *
             *          2 bytes: code (uint16)
             *          4 bytes: value (float32)
             */
            /** Metric ID type */
            typedef layout::base_metric< ::uint16_t > metric_id_t;
            /** Record type */
            typedef generic_layout<extended_tile_metric, 1> record_t;
            /** Code for each extended tile metric
             */
            enum ExtendedTileMetricCode
            {
                ClusterCountOccupied
            };
            /** Tile metric code */
            ::uint16_t code;
            /** Tile metric value */
            float value;
            /** Read metric from the input stream
             *
             * @param stream input stream
             * @param metric destination metric
             * @return number of bytes read or total number of bytes written
             */
            template<class Stream, class Metric, class Header>
            static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
            {
                typedef is_same<Stream,std::ostream> is_stream_same;
                static_assert(!is_stream_same::value, "Function does not support output");
                record_t rec;
                std::streamsize count = stream_map< record_t >(stream, rec);
                if(!stream) return count;

                float val = rec.value;
                if( val != val ) val = 0;
                switch(rec.code)
                {
                    case ClusterCountOccupied:
                        metric.m_cluster_count_occupied = val;
                        break;
                    default:
                        INTEROP_THROW(bad_format_exception, "Unexpected tile code: "+
                                                   util::lexical_cast<std::string>(rec.code)+
                                                   " == "+
                                                   util::lexical_cast<std::string>(ClusterCountOccupied));
                };

                return count;
            }
            /** Write metric to the output stream
             *
             * @param stream output stream
             * @param metric source metric
             * @return number of bytes read or total number of bytes written
             */
            template<class Metric, class Header>
            static std::streamsize map_stream(std::ostream& stream, Metric& metric, Header&, const bool)
            {
                record_t rec;
                if(metric.m_cluster_count_occupied == metric.m_cluster_count_occupied)
                {
                    rec.value = metric.m_cluster_count_occupied;
                    rec.code = ClusterCountOccupied;
                    write_binary(stream, rec);
                }
                return stream.tellp();
            }
            /** Compute the size of a single metric record
             *
             * @return record size
             */
            static record_size_t compute_size(const extended_tile_metric::header_type&)
            {
                return static_cast< record_size_t >(
                        sizeof(metric_id_t)+sizeof(generic_layout<extended_tile_metric, 1>)
                );
            }
            /** Compute header size
             *
             * @return header size
             */
            static record_size_t compute_header_size(const extended_tile_metric::header_type&)
            {
                return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
            }
        };

        /** Extended Tile Metric Record Layout Version 2
         *
         * This class provides an interface to reading the extended tile metric file:
         *  - InterOp/ExtendedTileMetrics.bin
         *  - InterOp/ExtendedTileMetricsOut.bin
         *
         * The class takes two template arguments:
         *
         *      1. Metric Type: extended_tile_metric
         *      2. Version: 2
         */
        template<>
        struct generic_layout<extended_tile_metric, 2> : public default_layout<2>
        {
            /** @page extended_tile_v2 Extended Tile Version 2
             *
             * This class provides an interface to reading the extended tile metric file:
             *  - InterOp/ExtendedTileMetrics.bin
             *  - InterOp/ExtendedTileMetricsOut.bin
             *
             *  The file format for extended tile metrics is as follows:
             *
             *  @b Header
             *
             *  illumina::interop::io::metric_format_stream (Class that parses this information)
             *
             *          byte 0: version number (2)
             *          byte 1: record size (10)
             *
             *  @b n-Records
             *
             *  illumina::interop::io::layout::base_metric (Class that parses this information)
             *
             *          2 bytes: lane number (uint16)
             *          4 bytes: tile number (uint32)
             *
             *  illumina::interop::io::generic_layout<extended_tile_metric, 2> (Class that parses this information)
             *
             *          4 bytes: cluster count occupied (float32)
             */

            /** Metric ID type */
            typedef layout::base_metric< ::uint32_t > metric_id_t;
            typedef float cluster_count_t;
            /** Map reading/writing to stream
             *
             * Reading and writing are symmetric operations, map it once
             *
             * @param stream input/output stream
             * @param metric source/destination metric
             * @return number of bytes read or total number of bytes written
             */
            template<class Stream, class Metric, class Header>
            static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
            {
                std::streamsize count = 0;
                count += stream_map< cluster_count_t >(stream, metric.m_cluster_count_occupied);
                return count;
            }
            /** Compute the layout size
             *
             * @return size of the record
             */
            static record_size_t compute_size(const extended_tile_metric::header_type&)
            {
                return static_cast<record_size_t>(sizeof(metric_id_t)+
                                                  sizeof(cluster_count_t)        // m_cluster_count_occupied
                );
            }
            /** Compute header size
             *
             * @return header size
             */
            static record_size_t compute_header_size(const extended_tile_metric::header_type&)
            {
                return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
            }
        };

    /** Extended Tile Metric Record Layout Version 3
     *
     * This class provides an interface to reading the extended tile metric file:
     *  - InterOp/ExtendedTileMetrics.bin
     *  - InterOp/ExtendedTileMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: extended_tile_metric
     *      2. Version: 3
     */
    template<>
    struct generic_layout<extended_tile_metric, 3> : public default_layout<3>
    {
        /** @page extended_tile_v3 Extended Tile Version 3
         *
         * This class provides an interface to reading the extended tile metric file:
         *  - InterOp/ExtendedTileMetrics.bin
         *  - InterOp/ExtendedTileMetricsOut.bin
         *
         *  The file format for extended tile metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::metric_format_stream (Class that parses this information)
         *
         *          byte 0: version number (3)
         *          byte 1: record size (10)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          4 bytes: tile number (uint32)
         *
         *  illumina::interop::io::generic_layout<extended_tile_metric, 3> (Class that parses this information)
         *
         *          4 bytes: cluster count occupied (float32)
         *          4 bytes: upper left location in x (float32)
         *          4 bytes: upper left location in y (float32)
         */

        /** Metric ID type */
        typedef layout::base_metric< ::uint32_t > metric_id_t;
        typedef float cluster_count_t;
        typedef float coordinate_t;
        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
        {
            std::streamsize count = 0;
            count += stream_map< cluster_count_t >(stream, metric.m_cluster_count_occupied);
            count += stream_map< coordinate_t >(stream, metric.m_upper_left.m_x);
            count += stream_map< coordinate_t >(stream, metric.m_upper_left.m_y);
            return count;
        }
        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const extended_tile_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t)+
            sizeof(cluster_count_t) +        // m_cluster_count_occupied
            sizeof(coordinate_t) +           // upper_left x
            sizeof(coordinate_t)             // upper_left y
            );
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const extended_tile_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };
#pragma pack()
    /** Extended tile Metric CSV text format
     *
     * This class provide an interface for writing the extended tile metrics to a CSV file:
     *
     *  - ExtendedTileMetrics.csv
     */
    template<>
    struct text_layout< extended_tile_metric, 1 >
    {
        /** Define a header type */
        typedef extended_tile_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type&,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* headers[] =
            {
                "Lane", "Tile", "OccupiedCount"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a extended tile metric to the output stream
         *
         * @param out output stream
         * @param metric extended tile metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const extended_tile_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.lane() << sep << metric.tile() << sep << metric.cluster_count_occupied();
            out << eol;
            return 0;
        }
    };
    /** Extended tile Metric CSV text format
     *
     * This class provide an interface for writing the extended tile metrics to a CSV file:
     *
     *  - ExtendedTileMetrics.csv
     */
    template<>
    struct text_layout< extended_tile_metric, 2 >
    {
        /** Define a header type */
        typedef extended_tile_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type&,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* headers[] =
            {
                "Lane", "Tile", "OccupiedCount", "Upper Left X", "Upper Left Y"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a extended tile metric to the output stream
         *
         * @param out output stream
         * @param metric extended tile metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const extended_tile_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.lane() << sep << metric.tile() << sep << metric.cluster_count_occupied();
            out << sep << metric.upper_left().x();
            out << sep << metric.upper_left().y();
            out << eol;
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(extended_tile_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extended_tile_metric, 1 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extended_tile_metric, 2 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extended_tile_metric, 3 )

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(extended_tile_metric, 1)
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(extended_tile_metric, 2)

