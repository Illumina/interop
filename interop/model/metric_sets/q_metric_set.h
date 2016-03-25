/** Q-score Metric Set
 *
 *  @file
 *
 *  @date 10/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/q_metric.h"
#include "interop/constants/enums.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {


/** Q-score Metric Set
 */
class q_metrics : public metric_base::metric_set<q_metric>
{
    typedef metric_base::metric_set<q_metric> parent_type;

public:
    /** Unsigned integer type */
    typedef q_metric::uint_t uint_t;
public:
    /** Constructor
     */
    q_metrics() : parent_type(header_type()), m_max_cycle(0)
    {
    }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    q_metrics(const metric_array_t& vec, const ::int16_t version=-1, const header_type& header=header_type()) :
            parent_type(vec, version, header), m_max_cycle(0) {
    }

public:
    /** Get maximum number of cycles
     *
     * @return maximum cycle
     */
    uint_t max_cycle()const
    {
        return m_max_cycle;
    }
    /** Get the number of bins for each metric
     *
     * @return number of bins for each metric
     */
    size_t histBinCount()const
    {
        return size() > 0 ? m_data[0].size() : 0;
    }
    /** Populate the cumulative distributions
     */
    void populateCumulativeDistributions() _INTEROP_MODEL_THROWS
    {
        if(size() > 0)
        {
            for(metric_array_t::iterator cur = m_data.begin(), end = m_data.end();cur != end;++cur)
                m_max_cycle = std::max(m_max_cycle, cur->cycle());
            id_vector lane_ids = lanes();
            for(id_vector::const_iterator lane_beg = lane_ids.begin(), lane_end = lane_ids.end();lane_beg != lane_end;++lane_beg)
            {
                id_vector tile_ids = tile_numbers_for_lane(*lane_beg);
                for(id_vector::const_iterator tile_beg = tile_ids.begin(), tile_end = tile_ids.end();tile_beg != tile_end;++tile_beg)
                {
                    if(!has_metric(*lane_beg, *tile_beg, 1)) continue;
                    q_metric& metric = get_metric_ref(*lane_beg, *tile_beg, 1);
                    metric.accumulate(metric);
                    for(uint_t cycle = 2;cycle <= m_max_cycle;++cycle)
                    {
                        if(!has_metric(*lane_beg, *tile_beg, cycle) || !has_metric(*lane_beg, *tile_beg, cycle-1)) continue;// TODO: if this happens zero out following q-scores
                        get_metric_ref(*lane_beg, *tile_beg, cycle).accumulate(get_metric_ref(*lane_beg, *tile_beg, cycle-1));
                    }
                }
            }
        }
    }
    /** Populate the q-score header bins from the data
     *
     * This only for legacy platforms that use older q-metric formats, which do not include bin information
     * in the header.
     *
     * @param instrument type
     */
    void build_bins(int32_t instrument)
    {
        if(binCount() > 0) return;
        if(size()==0) return;
        std::set<uint_t> bins_found;
        for(const_iterator b = m_data.begin(), e = m_data.end();b != e;++b)
        {
            for(uint_t i=0;i<static_cast<uint_t>(b->qscoreHist().size());++i)
                if(b->qscoreHist()[i] > 0)bins_found.insert(i);
        }
        const size_t count = bins_found.size() - (bins_found.count(0) > 0 ? 1 : 0);
        if(count > 7) return;
        if(instrument == constants::NextSeq || instrument==constants::MiniSeq)
        {
            m_qscoreBins.push_back(q_score_bin(0, 9, 8));
            m_qscoreBins.push_back(q_score_bin(10, 19, 13));
            m_qscoreBins.push_back(q_score_bin(20, 24, 22));
            m_qscoreBins.push_back(q_score_bin(25, 29, 27));
            m_qscoreBins.push_back(q_score_bin(30, 34, 32));
            m_qscoreBins.push_back(q_score_bin(35, 39, 37));
        }
        else if(count == 7)
        {
            m_qscoreBins.push_back(q_score_bin(0,   9,  6));
            m_qscoreBins.push_back(q_score_bin(10, 19, 15));
            m_qscoreBins.push_back(q_score_bin(20, 24, 22));
            m_qscoreBins.push_back(q_score_bin(25, 29, 27));
            m_qscoreBins.push_back(q_score_bin(30, 34, 33));
            m_qscoreBins.push_back(q_score_bin(35, 39, 37));
            m_qscoreBins.push_back(q_score_bin(40, 49, 40));
        }
        else if(count == 6)
        {
            m_qscoreBins.push_back(q_score_bin(0, 9, 7));
            m_qscoreBins.push_back(q_score_bin(10, 19, 16));
            m_qscoreBins.push_back(q_score_bin(20, 26, 24));
            m_qscoreBins.push_back(q_score_bin(27, 29, 29));
            m_qscoreBins.push_back(q_score_bin(30, 34, 33));
            m_qscoreBins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 5)
        {
            m_qscoreBins.push_back(q_score_bin(0,   9, 7));
            m_qscoreBins.push_back(q_score_bin(10, 19, 16));
            m_qscoreBins.push_back(q_score_bin(20, 29, 25));
            m_qscoreBins.push_back(q_score_bin(30, 34, 33));
            m_qscoreBins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 4)
        {
            m_qscoreBins.push_back(q_score_bin(0,   9, 7));
            m_qscoreBins.push_back(q_score_bin(10, 29, 20));
            m_qscoreBins.push_back(q_score_bin(30, 34, 33));
            m_qscoreBins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 3)
        {
            m_qscoreBins.push_back(q_score_bin(0,   9, 7));
            m_qscoreBins.push_back(q_score_bin(10, 29, 20));
            m_qscoreBins.push_back(q_score_bin(30, 49, 36));
        }
        else if(count == 2)
        {
            m_qscoreBins.push_back(q_score_bin(0, 27, 13));
            m_qscoreBins.push_back(q_score_bin(28, 49, 35));
        }
        else
        {
            m_qscoreBins.push_back(q_score_bin(0, 50, 20));
        }
    }

private:
    uint_t m_max_cycle;
};
}
}
}
}

