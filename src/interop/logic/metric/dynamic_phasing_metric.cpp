/** Logic for dynamic phasing metrics
 *
 *  @file
 *  @date  7/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/metric/dynamic_phasing_metric.h"
#include "interop/util/map.h"

namespace illumina { namespace interop { namespace logic { namespace metric
{
    /** Compute the linear fit online
     */
    class linear_fit
    {
    public:
        /** Constructor */
        linear_fit() : m_sx(0), m_sy(0), m_sxy(0), m_sxx(0), m_sample_count(0){}

    public:
        /** Add a point to the linear fit model
         *
         * @param x x-coordinate
         * @param y y-coordinate
         */
        void add(const float x, const float y)
        {
            m_sx += x;
            m_sy += y;
            m_sxy += x*y;
            m_sxx += x*x;
            m_sample_count++;
        }

        /** Slope of the line
         *
         * @return slope of the line
         */
        float slope()const
        {
            const float den = denominator();
            if (den <= std::numeric_limits<float>::epsilon())
                return std::numeric_limits<float>::quiet_NaN();
            return (m_sample_count * m_sxy - m_sx * m_sy) / den;
        }

        /** Offset of the line
         *
         * @return offset of the line
         */
        float offset()const
        {
            const float den = denominator();
            if (den <= std::numeric_limits<float>::epsilon())
                return std::numeric_limits<float>::quiet_NaN();
            return (m_sy * m_sxx - m_sx * m_sxy) / den;
        }
        /** Get the current sample count
         *
         * @return current sample count
         */
        size_t sample_count()const
        {
            return m_sample_count;
        }

    private:
        float denominator()const
        {
            return  m_sample_count * m_sxx - m_sx * m_sx;
        }

    private:
        float m_sx;
        float m_sy;
        float m_sxy;
        float m_sxx;
        size_t m_sample_count;
    };

    /** Container for both phasing and prephasing linear fits
     */
    struct phasing_prephasing_fit
    {
        typedef model::metric_base::base_read_metric::uint_t uint_t;
        /** Constructor
         *
         * @param id_ base metric id
         * @param read read number
         */
        phasing_prephasing_fit(const model::metric_base::base_metric& id_, const size_t read) :
                id(model::metric_base::base_read_metric(id_.lane(), id_.tile(), static_cast<uint_t>(read))){}
        /** Phasing fit */
        linear_fit phasing;
        /** Prephasing fit */
        linear_fit prehasing;
        /** Id for value */
        model::metric_base::base_read_metric id;
    };

    /** Populate the dynamic phasing metrics (slope & offset for phasing/prephasing) data structure given the phasing metrics data
     *
     * @param phasing_metrics phasing metric set
     * @param cycle_to_read map of cycle to read information
     * @param dynamic_phasing_metrics dynamic phasing metric set (to be populated)
     */
    void populate_dynamic_phasing_metrics(const model::metric_base::metric_set<model::metrics::phasing_metric>& phasing_metrics,
                                          const logic::summary::read_cycle_vector_t& cycle_to_read,
                                          model::metric_base::metric_set<model::metrics::dynamic_phasing_metric>& dynamic_phasing_metrics,
                                          model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics)
    {
        typedef INTEROP_UNORDERED_MAP(model::metric_base::base_metric::id_t, phasing_prephasing_fit) lookup_map_t;
        typedef model::metric_base::metric_set<model::metrics::phasing_metric> phasing_metric_set_t;
        typedef model::metrics::dynamic_phasing_metric::uint_t uint_t;
        typedef phasing_metric_set_t::const_iterator const_iterator;
        if (phasing_metrics.size() == 0)return;
        if (phasing_metrics.max_cycle() < 25) return;
        lookup_map_t fit_map;
        for(const_iterator beg = phasing_metrics.begin();beg != phasing_metrics.end();++beg)
        {
            const uint_t lane = beg->lane();
            const uint_t tile = beg->tile();
            INTEROP_ASSERT(beg->cycle()-1 < cycle_to_read.size());
            logic::summary::read_cycle_vector_t::const_reference read = cycle_to_read[beg->cycle()-1];
            const model::metric_base::base_read_metric::id_t id = model::metric_base::base_read_metric::create_id(lane, tile, read.number);
            lookup_map_t::iterator fit_for_tile = fit_map.find(id);
            if(fit_for_tile == fit_map.end())
            {
                fit_for_tile = fit_map.insert(lookup_map_t::value_type(id, phasing_prephasing_fit(*beg, read.number))).first;
            }
            const float cycle_within_read = static_cast<float>(read.cycle_within_read);
            fit_for_tile->second.phasing.add(cycle_within_read, beg->phasing_weight());
            fit_for_tile->second.prehasing.add(cycle_within_read, beg->prephasing_weight());
            if(fit_for_tile->second.phasing.sample_count() == 25)
            {
                const size_t tile_offset = tile_metrics.find(lane, tile);
                if(tile_offset < tile_metrics.size())
                {
                    model::metrics::tile_metric& tile_metric = tile_metrics[tile_offset];
                    tile_metric.update_phasing_if_missing(read.number,
                                                          fit_for_tile->second.phasing.slope(),
                                                          fit_for_tile->second.prehasing.slope());
                }
            }
        }
        for(lookup_map_t::const_iterator beg = fit_map.begin();beg != fit_map.end();++beg)
        {
            dynamic_phasing_metrics.insert(model::metrics::dynamic_phasing_metric(
                    beg->second.id.lane(), beg->second.id.tile(), beg->second.id.read(),
                    beg->second.phasing.slope(),
                    beg->second.phasing.offset(),
                    beg->second.prehasing.slope(),
                    beg->second.prehasing.offset()
            ));
        }
    }

}}}}

