/** Index Metric Set
 *
 *  @file
 *
 *  @date 11/4/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/index_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {



/** Index Metric Set
 */
class index_metrics : public metric_base::metric_set<index_metric>
{
    typedef metric_base::metric_set<index_metric> parent_type;

public:
    /** Vector of strings */
    typedef std::vector<std::string> string_vector;
public:
    /** Constructor
     */
    index_metrics() : parent_type(header_type())
    { }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    index_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type()) :
            parent_type(vec, version, header)
    {
    }
    /** The metric at the underlying index has changed, update the
     * metric set.
     *
     * This is called by illumina::interop::io::metric_format_stream::read_metrics_static every
     * time a metric record is read from a stream.
     *
     * @param index index of metric updated
     */
    void metric_updated_at(const size_t index)
    {
        const metric_type::index_array_t& indicies = parent_type::at(index).indices();
        for(metric_type::index_array_t::const_iterator beg = indicies.begin(), end = indicies.end(); beg != end; ++beg)
        {
            if (std::find(m_indicies.begin(), m_indicies.end(), beg->index_seq()) == m_indicies.end())
                m_indicies.push_back(beg->index_seq());
        }
    }

public:
    /** Get list of index sequences
     *
     * @return list of index sequences
     */
    const string_vector& indices()const
    {
        return m_indicies;
    }

private:
    string_vector m_indicies;
};
}
}
}
}