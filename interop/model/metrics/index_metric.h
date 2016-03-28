/** Index metric
 *
 * The index metric holds string information describing the indexes used in the run.
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/IndexMetrics.bin
 *  - InterOp/IndexMetricsOut.bin
 *
 *  @file
 *
 *  @date 11/2/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <assert.h>
#include <string>
#include <map>
#include "interop/model/metric_base/base_read_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {

/** Index information
 *
 * This class defines all the information that describes an index within a sequencing run.
 *
 * @note Supported versions: 1
 */
class index_info
{
public:
    enum{
        /** Unique type code for metric */
        TYPE=constants::Index,
        /** Latest version of the InterOp format */
        LATEST_VERSION=1
    };
public:
    /** Constructor
     *
     */
    index_info() :
            m_index_seq(""),
            m_sample_id(""),
            m_sample_proj(""),
            m_count(0)
    {
    }
    /** Constructor
     *
     * @param index_seq index sequence
     * @param sample_id sample id
     * @param sample_proj sample project
     * @param count number of index sequences
     */
    index_info(const std::string& index_seq,
               const std::string& sample_id,
               const std::string& sample_proj,
               size_t count) :
            m_index_seq(index_seq),
            m_sample_id(sample_id),
            m_sample_proj(sample_proj),
            m_count(count)
    {
    }

public:
    /** @defgroup index_info Index Info
     *
     *
     * @ref illumina::interop::model::metrics::index_info "See full class description"
     * @{
     */
    /** Get the index sequence
     *
     * @return index sequence
     */
    const std::string& index_seq()const{return m_index_seq;}
    /** Get the sample id
     *
     * @return sample id
     */
    const std::string& sample_id()const{return m_sample_id;}
    /** Get the sample project
     *
     * @return sample project
     */
    const std::string& sample_proj()const{return m_sample_proj;}
    /** Get the number of index sequences
     *
     * @return number of index sequences
     */
    size_t count()const{return m_count;}
    /** @} */

private:
    std::string m_index_seq;
    std::string m_sample_id;
    std::string m_sample_proj;
    size_t m_count;
    template<class MetricType, int Version>
    friend struct io::generic_layout;
};

/** Index metric
 *
 * The index metric holds string information describing the indexes used in the run.
 *
 * @note Supported versions: 1
 */
class index_metric : public metric_base::base_read_metric
{
public:
    /** Define a index array using an underlying vector
     */
    typedef std::vector<index_info> index_array_t;
    /** Define index info type
     */
    typedef index_info index_info_t;
public:
    /** Constructor
     */
    index_metric() :
            metric_base::base_read_metric(0,0,0)
    {
    }
    /** Constructor
     *
     * @param lane lane number
     * @param tile tile number
     * @param read read number
     * @param indices array of indexes
     */
    index_metric(uint_t lane,
                 uint_t tile,
                 uint_t read,
                 const index_array_t& indices) :
            metric_base::base_read_metric(lane,tile,read),
            m_indices(indices)
    {
    }

public:
    /** @defgroup index_metric Index Metrics
     *
     *
     * @ref illumina::interop::model::metrics::index_metric "See full class description"
     * @{
     */
    /** Get the number of indexes
     *
     * @return number of indexes.
     */
    size_t size()const
    {
        return m_indices.size();
    }
    /** Get index info at given position
     *
     * @param n index of index
     * @return inex
     */
    const index_info& indices(size_t n)const
    {
        assert(n<m_indices.size());
        return m_indices[n];
    }
    /** Get an array of indices
     *
     * @return vector of indices
     */
    const index_array_t& indices()const
    {
        return m_indices;
    }
    /** @} */

public:
    /** Get the prefix of the InterOp filename
     *
     * @return "Index"
     */
    static const char* prefix(){return "Index";}

private:
    index_array_t m_indices;
    template<class MetricType, int Version>
    friend struct io::generic_layout;
};
}
}
}
}
