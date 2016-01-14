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
    /** Constructor
     */
    q_metrics() : parent_type(header_type())
    {
    }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    q_metrics(const metric_array_t& vec, const ::int16_t version=-1, const header_type& header=header_type()) :
            parent_type(vec, version, header) {
    }

public:
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
    void populateCumulativeDistributions()
    {
        if(size() > 0)
        {
            m_data[0].accumulate(m_data[0]);
            for(metric_array_t::iterator beg = m_data.begin(), cur=beg+1, end = m_data.end();cur != end;++beg,++cur)
            {
                if(cur->lane() == (cur-1)->lane() && cur->tile() == (cur-1)->tile())
                    cur->accumulate(*beg);
                else cur->accumulate(*cur);
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
            bins_found.insert(b->qscoreHist().begin(), b->qscoreHist().end());
        size_t count = bins_found.size() - (bins_found.count(0) > 0 ? 1 : 0);
        if(count > 7) return;
        if(instrument == constants::NextSeq)
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
};
}
}
}
}