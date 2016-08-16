/** Metric set holds a collection metrics
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "interop/util/exception.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/base_read_metric.h"
#include "interop/model/model_exceptions.h"
#include "interop/util/lexical_cast.h"
#include "interop/util/assert.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

namespace illumina { namespace interop { namespace model { namespace metric_base
{

    /** Metric set holds a collection metrics
     *
     * This class holds a map that maps a unique id to the metric.
     */
    template<typename T>
    class metric_set : public T::header_type
    {
    public:
        /** Define a collection of metrics
         */
        typedef std::vector<T> metric_array_t;
        /** Define a write header */
        typedef typename T::header_type header_type;
        /** Define the base type for the metric */
        typedef typename T::base_t base_t;
        /** Define a metric type */
        typedef T metric_type;
        /** Define a metric set type */
        typedef metric_set<T> metric_set_type;
        /** Define a ID type */
        typedef typename T::id_t id_t;
        /** Define a lane/tile/cycle id type  */
        typedef typename T::uint_t uint_t;
        /** Define an id vector */
        typedef std::vector<uint_t> id_vector;
        /** Define an key vector */
        typedef std::vector<id_t> key_vector;
        /** Define the size type */
        typedef typename metric_array_t::size_type size_type;

    public:
        /** Const metric iterator */
        typedef typename metric_array_t::const_iterator const_iterator;
        /** Metric iterator */
        typedef typename metric_array_t::iterator iterator;
    private:
        typedef std::map<id_t, size_t> id_map_t;

    public:
        /** Constructor
         *
         * @param version version of the file format
         */
        metric_set(const ::int16_t version )
                : header_type(header_type::default_header()), m_version(version)
        { }
        /** Constructor
         *
         * @param header header information for the metric set
         * @param version version of the file format
         */
        metric_set(const header_type &header = header_type::default_header(), const ::int16_t version = 0)
                : header_type(header), m_version(version)
        { }

        /** Constructor
         *
         * @param vec array of metrics
         * @param version of the metric format
         * @param header header information for the metric set
         */
        metric_set(const metric_array_t &vec, const ::int16_t version, const header_type &header) :
                header_type(header),
                m_data(vec),
                m_version(version)
        {
            size_t offset = 0;
            for (typename metric_array_t::const_iterator b = vec.begin(), e = vec.end(); b != e; ++b)
            {
                m_id_map[b->id()] = offset;
                ++offset;
                T::header_type::update_max_cycle(*b);
            }
        }

    public:
        /** Get start of metric collection
         *
         * @return iterator to start of metric collection
         */
        const_iterator begin() const
        {
            return m_data.begin();
        }

        /** Get end of metric collection
         *
         * @return iterator to end of metric collection
         */
        const_iterator end() const
        {
            return m_data.end();
        }

        /** Get start of metric collection
         *
         * @return iterator to start of metric collection
         */
        iterator begin()
        {
            return m_data.begin();
        }

        /** Get end of metric collection
         *
         * @return iterator to end of metric collection
         */
        iterator end()
        {
            return m_data.end();
        }

    public:
        /** Find index of metric given the id. If not found, return number of metrics
         *
         * @param lane lane
         * @param tile tile
         * @param cycle cycle
         * @return index of metric or number of metrics
         */
        size_t find(const uint_t lane, const uint_t tile, const uint_t cycle = 0) const
        {
            return find(metric_type::create_id(lane, tile, cycle));
        }

        /** Find index of metric given the id. If not found, return number of metrics
         *
         * @param id id
         * @return index of metric or number of metrics
         */
        size_t find(const id_t id) const
        {
            typename id_map_t::const_iterator it = m_id_map.find(id);
            if (it == m_id_map.end()) return size();
            return it->second;
        }

        /** Test if set has metric
         *
         * @param lane lane
         * @param tile tile
         * @param cycle cycle
         */
        bool has_metric(const uint_t lane, const uint_t tile, const uint_t cycle = 0) const
        {
            return has_metric(metric_type::create_id(lane, tile, cycle));
        }

        /** Test if set has metric
         *
         * @param id id
         */
        bool has_metric(const id_t id) const
        {
            return m_id_map.find(id) != m_id_map.end();
        }

        /** Add a metric to the metric set
         *
         * @param metric metric to add to set
         */
        void insert(const metric_type &metric)
        {
            insert(metric.id(), metric);
        }

        /** Add a metric to the metric set
         *
         * @param id unique id for metric
         * @param metric metric to add to set
         */
        void insert(const id_t id, const metric_type &metric)
        {
            INTEROP_ASSERT(id != 0);
            // TODO: remove the following
            m_id_map[id] = m_data.size();

            T::header_type::update_max_cycle(metric);
            m_data.push_back(metric);
        }

        /** Get metric for lane, tile and cycle
         *
         * @todo: remove this function
         *
         * @param lane lane
         * @param tile tile
         * @param cycle cycle
         * @return metric
         */
        const metric_type &get_metric(uint_t lane, uint_t tile,
                                      uint_t cycle = 0) const throw(model::index_out_of_bounds_exception)
        {
            try
            {
                return get_metric(metric_type::create_id(lane, tile, cycle));
            }
            catch (const index_out_of_bounds_exception &)
            {
                INTEROP_THROW( index_out_of_bounds_exception, "No tile available: key: " <<
                                       metric_type::create_id(lane, tile, cycle) <<
                                                    " map: " << (m_id_map.size()) <<
                                                    "  lane: " << (lane) <<
                                                    "  tile: " << (tile) <<
                                                    "  cycle: " << (cycle));

            }
        }

        /** Get metric for a unique identifier
         *
         * @todo: remove this function
         *
         * @param key unique id built from lane, tile and cycle (if available)
         * @return metric
         */
        const metric_type &get_metric(id_t key) const throw(model::index_out_of_bounds_exception)
        {
            typename std::map<id_t, size_t>::const_iterator it = m_id_map.find(key);
            if (it == m_id_map.end())
                INTEROP_THROW( index_out_of_bounds_exception, "No tile available: key: " <<  key << " map: " <<
                        m_id_map.size() << " == data: " <<
                        m_data.size());
            INTEROP_ASSERT(it->second < m_data.size());
            return m_data[it->second];
        }

        /** Get a metric at the given index
         *
         * @param n index
         * @return metric
         */
        metric_type &at(size_t n) throw(index_out_of_bounds_exception)
        {
            if(n >= m_data.size()) INTEROP_THROW(index_out_of_bounds_exception, "Index out of bounds");
            return m_data.at(n);
        }

        /** Set the version of the InterOp file
         *
         * The InterOp file version determines which metric fields are populated. Each
         * metric lists which version is supported.
         *
         * @todo make this private
         *
         * @param version version number
         */
        void set_version(const ::int16_t version)
        {
            m_version = version;
        }

        /** Get a list of all available lane numbers
         *
         * @return vector of lane numbers
         */
        key_vector keys() const
        {
            key_vector ids;
            std::transform(m_data.begin(), m_data.end(), std::back_inserter(ids), to_id);
            return ids;
        }

        /** Get a list of all available lane numbers
         *
         * @return vector of lane numbers
         */
        id_vector lanes() const
        {
            std::set<uint_t> id_set;
            std::transform(m_data.begin(), m_data.end(), std::inserter(id_set, id_set.begin()), to_lane);
            return id_vector(id_set.begin(), id_set.end());
        }

        /** Get the number of lanes in the data
         *
         * @return number of lanes
         */
        size_t lane_count() const
        {
            return lanes().size();
        }

        /** Get maximum lane number
         *
         * @return maximum lane number
         */
        size_t max_lane() const
        {
            size_t lane_max = 0;
            for (const_iterator b = m_data.begin(), e = m_data.end(); b != e; ++b)
                lane_max = std::max(lane_max, static_cast<size_t>(b->lane()));
            return lane_max;
        }

        /** Get a list of all available tile numbers for the specified lane
         *
         * @param lane lane number
         * @return vector of tile numbers
         */
        id_vector tile_numbers_for_lane(const uint_t lane) const
        {
            std::set<uint_t> tile_number_set;
            transform_if(m_data.begin(),
                         m_data.end(),
                         std::inserter(tile_number_set, tile_number_set.begin()),
                         lane_equals(lane),
                         to_tile);
            id_vector tile_numbers(tile_number_set.begin(), tile_number_set.end());
            return tile_numbers;
        }

        /** Get a list of all available tile numbers
         *
         * @return vector of tile numbers
         */
        id_vector tile_numbers() const
        {
            std::set<uint_t> tile_number_set;
            transform(m_data.begin(),
                         m_data.end(),
                         std::inserter(tile_number_set, tile_number_set.begin()),
                         to_tile);
            id_vector tile_numbers(tile_number_set.begin(), tile_number_set.end());
            return tile_numbers;
        }

        /** Get a list of all available metrics for the specified lane
         *
         * @param lane lane number
         * @return vector of tile numbers
         */
        metric_array_t metrics_for_lane(const uint_t lane) const
        {
            metric_array_t lane_metrics;
            lane_metrics.reserve(size());
            copy_if(m_data.begin(),
                    m_data.end(),
                    std::back_inserter(lane_metrics),
                    lane_equals(lane));
            metric_array_t(lane_metrics).swap(lane_metrics); // Shrink to fit
            return lane_metrics;
        }

        /** Get a list of all available metrics for the specified cycle
         *
         * @note Returns empty array for Tile Metrics, which does not contain a cycle identifier
         * @param cycle cycle number
         * @return vector of tile numbers
         */
        metric_array_t metrics_for_cycle(const uint_t cycle) const
        {
            return metrics_for_cycle(cycle, (typename T::header_type *) 0);
        }

    public:
        /** Number of metrics in the metric set
         *
         * @return number of metrics
         */
        size_t size() const
        { return m_data.size(); }
        /** Test if metric set is empty
         *
         * @return true if metric set is empty
         */
        bool empty() const
        { return m_data.empty(); }

        /** Version of the InterOp file parsed
         *
         * The InterOp file version determines which metric fields are populated. Each
         * metric lists which version is supported.
         *
         * @return interop file version
         */
        ::int16_t version() const
        {
            return m_version;
        }

        /** Clear the metrics in the metric set
         */
        void clear()
        {
            m_id_map.clear();
            m_data.clear();
        }

        /** Get the metrics in a vector
         *
         * @return vector of metrics
         */
        const metric_array_t &metrics() const
        {
            return m_data;
        }

        /** Get the prefix of the InterOp filename
         *
         * @return prefix
         */
        static const char *prefix()
        { return T::prefix(); }

        /** Get the suffix of the InterOp filename
         *
         * @return suffix
         */
        static const char *suffix()
        { return T::suffix(); }

        /** The metric at the underlying index has changed, update the
         * metric set.
         *
         * By default, this method does nothing.
         *
         * This is called by illumina::interop::io::metric_format_stream::read_metrics_static every
         * time a metric record is read from a stream.
         */
        void metric_updated_at(const size_t)
        { }

    public:
        /** Get metric for lane, tile and cycle
         *
         * @todo: remove this function
         *
         * @param lane lane
         * @param tile tile
         * @param cycle cycle
         * @return metric
         */
        metric_type &get_metric_ref(uint_t lane, uint_t tile,
                                    uint_t cycle = 0) throw(model::index_out_of_bounds_exception)
        {
            try
            {
                return get_metric_ref(metric_type::create_id(lane, tile, cycle));
            }
            catch (const index_out_of_bounds_exception &)
            {
                INTEROP_THROW( index_out_of_bounds_exception,"No tile available: key: " <<
                                                    metric_type::create_id(lane, tile, cycle) <<
                                                    " map: " <<(m_id_map.size()) <<
                                                    "  lane: " << (lane) <<
                                                    "  tile: " << (tile) <<
                                                    "  cycle: " << (cycle));

            }
        }

        /** Get metric for a unique identifier
         *
         * @todo: remove this function
         *
         * @param key unique id built from lane, tile and cycle (if available)
         * @return metric
         */
        metric_type &get_metric_ref(id_t key) throw(model::index_out_of_bounds_exception)
        {
            typename std::map<id_t, size_t>::const_iterator it = m_id_map.find(key);
            if (it == m_id_map.end())
                INTEROP_THROW( index_out_of_bounds_exception,
                        "No tile available: key: " << (key) << " map: " <<
                        (m_id_map.size()) << " == data: " <<
                        (m_data.size()));
            INTEROP_ASSERT(it->second < m_data.size());
            return m_data[it->second];
        }

    private:
        metric_array_t metrics_for_cycle(const uint_t cycle, base_cycle_metric_header *) const
        {
            metric_array_t cycle_metrics;
            cycle_metrics.reserve(size());
            copy_if(m_data.begin(),
                    m_data.end(),
                    std::back_inserter(cycle_metrics),
                    cycle_equals(cycle));
            metric_array_t(cycle_metrics).swap(cycle_metrics); // Shrink to fit
            return cycle_metrics;
        }

        metric_array_t metrics_for_cycle(const uint_t, base_metric_header *) const
        {
            return metric_array_t();
        }

        metric_array_t metrics_for_cycle(const uint_t, base_read_metric_header *) const
        {
            return metric_array_t();
        }


    private:
        static id_t to_id(const metric_type &metric)
        {
            return metric.id();
        }

        static uint_t to_lane(const metric_type &metric)
        {
            return metric.lane();
        }

        static uint_t to_tile(const metric_type &metric)
        {
            return metric.tile();
        }
        template<class I, class OIterator, class Operation>
        static OIterator transform(I beg, I end, OIterator it, Operation op)
        {
            while (beg != end)
            {
                *it++ = op(*beg);
                ++beg;
            }
            return it;
        }

        template<class I, class OIterator, class Predicate, class Operation>
        static OIterator transform_if(I beg, I end, OIterator it, Predicate pred, Operation op)
        {
            while (beg != end)
            {
                if (pred(*beg))
                    *it++ = op(*beg);
                ++beg;
            }
            return it;
        }

        template<class OIterator, class Predicate>
        static OIterator copy_if(const_iterator beg, const_iterator end, OIterator it, Predicate pred)
        {
            while (beg != end)
            {
                if (pred(*beg))
                    *it++ = *beg;
                ++beg;
            }
            return it;
        }

        struct lane_equals
        {
            lane_equals(uint_t lane) : m_lane(lane)
            { }

            bool operator()(const metric_type &metric) const
            { return metric.lane() == m_lane; }

            uint_t m_lane;
        };

        struct cycle_equals
        {
            cycle_equals(uint_t cycle) : m_cycle(cycle)
            { }

            bool operator()(const metric_type &metric) const
            { return metric.cycle() == m_cycle; }

            uint_t m_cycle;
        };

    protected:
        /** Array of metric data */
        metric_array_t m_data;
        /** Version of the metric read */
        ::int16_t m_version;

        // TODO: remove the following
        /** Map unique identifiers to the index of the metric */
        id_map_t m_id_map;
    };
}}}}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

