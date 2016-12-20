/** Register format layouts for tile metrics
 *
 * Each version of the tile metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include "interop/util/math.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Tile Metric Record Layout Version 2
     *
     * This class provides an interface to reading the tile metric file:
     *  - InterOp/TileMetrics.bin
     *  - InterOp/TileMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: tile_metric
     *      2. Version: 2
     */
    template<>
    struct generic_layout<tile_metric, 2> : public default_layout<2, 1 /*Multi record */>
    {
        /** @page tile_v2 Tile Version 2
         *
         * This class provides an interface to reading the tile metric file:
         *  - InterOp/TileMetrics.bin
         *  - InterOp/TileMetricsOut.bin
         *
         *  The file format for tile metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (10)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *
         *  illumina::interop::io::generic_layout<tile_metric, 2> (Class that parses this information)
         *
         *          2 bytes: code (uint16)
         *          4 bytes: value (float32)
         */
        /** Metric ID type */
        typedef layout::base_metric< ::uint16_t > metric_id_t;
        /** Record type */
        typedef generic_layout<tile_metric, 2> record_t;
        /** Code type */
        typedef ::uint16_t code_t;
        /** Value type */
        typedef float value_t;
        /** Code for each tile metric
         */
        enum TileMetricCode
        {
            ClusterDensity = 100,
            ClusterDensityPf = 101,
            ClusterCount = 102,
            ClusterCountPf = 103,
            Phasing = 200,
            Prephasing = 201,
            PercentAligned = 300,
            ControlLane = 400
        };
        /** Tile Metric Code */
        code_t code;
        /** Tile Metric Value */
        value_t value;

        /** Read metric from the input stream
         *
         * @param stream input stream
         * @param metric destination metric
         * @param is_new true if metric is new
         * @return number of bytes read or total number of bytes written
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::istream &stream, Metric &metric, Header &, const bool is_new)
        {
            record_t rec;
            std::streamsize count = stream_map<record_t>(stream, rec);
            if (stream.fail()) return count;
            float val = rec.value;
            if (val != val) val = 0; // TODO: Remove this after baseline
            switch (rec.code)
            {
                case ControlLane:
                    if (is_new) metric.set_base(metric_id_t());
                    break;
                case ClusterDensity:
                    metric.m_cluster_density = val;
                    break;
                case ClusterDensityPf:
                    metric.m_cluster_density_pf = val;
                    break;
                case ClusterCount:
                    metric.m_cluster_count = val;
                    break;
                case ClusterCountPf:
                    metric.m_cluster_count_pf = val;
                    break;
                default:
                    if (rec.code % Phasing < 100)
                    {
                        //code = Prephasing+read*2;
                        int code_offset = rec.code % Phasing;
                        if (code_offset % 2 == 0)
                        {
                            get_read(metric, (code_offset / 2) + 1)->percent_phasing(val * 100);
                        }
                        else
                        {
                            get_read(metric, (code_offset + 1) / 2)->percent_prephasing(val * 100);
                        }
                    }
                    else if (rec.code % PercentAligned < 100)
                    {
                        int code_offset = rec.code % PercentAligned;
                        get_read(metric, code_offset + 1)->percent_aligned(val);
                    }
                    else
                        INTEROP_THROW(bad_format_exception, "Unexpected tile code");
            };

            return count;
        }

        /** Write metric to the output stream
         *
         * @param out output stream
         * @param metric source metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::ostream &out, Metric &metric, Header &, const bool)
        {
            record_t rec;
            metric_id_t metric_id;
            metric_id.set(metric);
            bool write_id = false;

            // We always write this out, even if it is NaN
            // We always write out ID for the first record
            if (!std::isnan(metric.m_cluster_density))
            {
                rec.value = metric.m_cluster_density;
                rec.code = ClusterDensity;
                if (write_id) write_binary(out, metric_id);
                write_id = true;
                write_binary(out, rec);
            }
            if (!std::isnan(metric.m_cluster_density_pf))
            {
                rec.value = metric.m_cluster_density_pf;
                rec.code = ClusterDensityPf;
                if (write_id) write_binary(out, metric_id);
                write_id = true;
                write_binary(out, rec);
            }
            if (!std::isnan(metric.m_cluster_count))
            {
                rec.value = metric.m_cluster_count;
                rec.code = ClusterCount;
                if (write_id) write_binary(out, metric_id);
                write_id = true;
                write_binary(out, rec);
            }
            if (!std::isnan(metric.m_cluster_count_pf))
            {
                rec.value = metric.m_cluster_count_pf;
                rec.code = ClusterCountPf;
                if (write_id) write_binary(out, metric_id);
                write_id = true;
                write_binary(out, rec);
            }
            typedef tile_metric::read_metric_vector::const_iterator const_iterator;
            for (const_iterator beg = metric.read_metrics().begin(); beg != metric.read_metrics().end(); beg++)
            {
                const int read = beg->read() - 1;
                if(!std::isnan(beg->percent_prephasing()))
                {
                    rec.code = static_cast< code_t >(Prephasing + read * 2);
                    rec.value = beg->percent_prephasing();
                    if (write_id) write_binary(out, metric_id);
                    write_id = true;
                    write_binary(out, rec);
                }
                if(!std::isnan(beg->percent_phasing()))
                {
                    rec.code = static_cast< code_t >(Phasing + read * 2);
                    rec.value = beg->percent_phasing();
                    if (write_id) write_binary(out, metric_id);
                    write_id = true;
                    write_binary(out, rec);
                }
                if(!std::isnan(beg->percent_aligned()))
                {
                    rec.code = static_cast< code_t >(PercentAligned + read);
                    rec.value = beg->percent_aligned();
                    if (write_id) write_binary(out, metric_id);
                    write_id = true;
                    write_binary(out, rec);
                }
            }
            return out.tellp();
        }
        /** Throws an unimplemented error
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(const char*, const Metric &, const Header &, const bool)
        {
            INTEROP_THROW(std::runtime_error, "Function not implemented");
        }

        /** Compute the size of a single metric record
         *
         * @return record size
         */
        static record_size_t compute_size(const tile_metric::header_type &)
        {
            return static_cast< record_size_t >(sizeof(metric_id_t) + sizeof(record_t));
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const tile_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }

    private:
        static tile_metric::read_metric_vector::iterator get_read(tile_metric &metric,
                                                                  tile_metric::read_metric_type::uint_t read)
        {
            tile_metric::read_metric_vector::iterator it = metric.m_read_metrics.begin();
            for (; it != metric.m_read_metrics.end(); it++)
                if (it->read() == read) return it;
            metric.m_read_metrics.push_back(model::metrics::read_metric(read));
            return metric.m_read_metrics.begin() + metric.m_read_metrics.size() - 1;
        }
    };



#pragma pack()
    /** Tile Metric CSV text format
     *
     * This class provide an interface for writing the tile metrics to a CSV file:
     *
     *  - TileMetrics.csv
     */
    template<>
    struct text_layout< tile_metric, 1 >
    {
        /** Define a header type */
        typedef tile_metric::header_type header_type;
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
                "Lane", "Tile", "Read",
                "ClusterCount", "ClusterCountPF", "Density", "DensityPF", "Aligned", "Prephasing", "Phasing"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a tile metric to the output stream
         *
         * @param out output stream
         * @param metric tile metric
         * @param sep column separator
         * @param eol row separator
         * @param missing missing value indicator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const tile_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char missing)
        {
            if(metric.read_metrics().empty())
            {
                out << metric.lane() << sep << metric.tile() << sep << missing << sep;
                out << metric.cluster_count() <<  sep << metric.cluster_count_pf() << sep;
                out << metric.cluster_density() <<  sep<< metric.cluster_density_pf() << sep;
                out << missing << sep << missing <<sep << missing;
                out << eol;
            }
            else
            {
                for(tile_metric::read_metric_vector::const_iterator rbeg = metric.read_metrics().begin(),
                            rend = metric.read_metrics().end();rbeg != rend;++rbeg)
                {
                    out << metric.lane() << sep << metric.tile() << sep << rbeg->read() << sep;
                    out << metric.cluster_count() << sep << metric.cluster_count_pf() << sep;
                    out << metric.cluster_density() << sep << metric.cluster_density_pf() << sep;
                    out << rbeg->percent_aligned() << sep << rbeg->percent_prephasing()
                        << sep << rbeg->percent_phasing();
                    out << eol;
                }
            }
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(tile_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(tile_metric, 2)

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(tile_metric, 1)
