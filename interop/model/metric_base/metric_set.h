/** Metric set holds a collection metrics
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <set>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "interop/util/map.h"
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
        /** Define a set of ids */
        typedef std::set<uint_t> id_set_t; // TODO: Do the same for set
        /** Define offset map */
#if defined(__cplusplus) && __cplusplus < 201103L // Workaround for SWIG not understanding the macro
        typedef std::map<id_t, size_t> offset_map_t;
#else
        typedef std::unordered_map<id_t, size_t> offset_map_t;
#endif

    public:
        /** Const metric iterator */
        typedef typename metric_array_t::const_iterator const_iterator;
        /** Metric iterator */
        typedef typename metric_array_t::iterator iterator;
        enum
        {
            /** Group type enum */
            TYPE=metric_attributes<T>::TYPE,
            /** Latest version of the format */
            LATEST_VERSION=metric_attributes<T>::LATEST_VERSION
        };

    public:
        /** Constructor
         *
         * @param version version of the file format
         */
        metric_set(const ::int16_t version )
                : header_type(header_type::default_header()), m_version(version), m_data_source_exists(false)
        { }
        /** Constructor
         *
         * @param header header information for the metric set
         * @param version version of the file format
         */
        metric_set(const header_type &header = header_type::default_header(), const ::int16_t version = 0)
                : header_type(header), m_version(version), m_data_source_exists(false)
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
                m_version(version),
                m_data_source_exists(false)
        {
            rebuild_index(true);
        }

    public:
        /** Flag that indicates whether the data source exists
         *
         * @return true if the data source, e.g. file, exists
         */
        bool data_source_exists()const
        {
            return !empty() || m_data_source_exists;
        }
        /** Set flag that indicates whether the data source exists
         *
         * @param exists true if the data source, e.g. file, exists
         */
        void data_source_exists(const bool exists)
        {
            m_data_source_exists = exists;
        }
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
        /** Rebuild the index map and update the cycle state
         */
        void rebuild_index(const bool update_ids=false)
        {
            size_t offset = 0;
            for (const_iterator b = begin(), e = end(); b != e; ++b)
            {
                if(update_ids)
                {
                    m_id_map[b->id()] = offset;
                    ++offset;
                }
                T::header_type::update_max_cycle(*b);
            }
        }
        /** Resert the number of places in the metric vector
         *
         * @param n expected number of elements
         */
        void resize(const size_t n)
        {
            m_data.resize(n, metric_type(*this));
        }
        /** Trim the set to the proper number of metrics
         *
         * @param n actual size of the metric set
         */
        void trim(const size_t n)
        {
            m_data.resize(n);
        }
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
            typename offset_map_t::const_iterator it = m_id_map.find(id);
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
            m_id_map[id] = size();

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
        const metric_type &get_metric(const uint_t lane, const uint_t tile,
                                      const uint_t cycle = 0) const throw(model::index_out_of_bounds_exception)
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
        const metric_type &get_metric(const id_t key) const throw(model::index_out_of_bounds_exception)
        {
            typename offset_map_t::const_iterator it = m_id_map.find(key);
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
        metric_type &operator[](const size_t n) throw(index_out_of_bounds_exception)
        {
            if(n >= m_data.size()) INTEROP_THROW(index_out_of_bounds_exception, "Index out of bounds");
            return m_data[n];
        }

        /** Get a metric at the given index
         *
         * @param n index
         * @return metric
         */
        const metric_type &operator[](const size_t n)const throw(index_out_of_bounds_exception)
        {
            if(n >= m_data.size()) INTEROP_THROW(index_out_of_bounds_exception, "Index out of bounds");
            return m_data[n];
        }

        /** Get a metric at the given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return metric
         */
        metric_type &at(const size_t n) throw(index_out_of_bounds_exception)
        {
            if(n >= m_data.size()) INTEROP_THROW(index_out_of_bounds_exception, "Index out of bounds");
            return m_data[n];
        }

        /** Get a metric at the given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return metric
         */
        const metric_type &at(const size_t n)const throw(index_out_of_bounds_exception)
        {
            if(n >= m_data.size()) INTEROP_THROW(index_out_of_bounds_exception, "Index out of bounds");
            return m_data[n];
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
            std::transform(begin(), end(), std::back_inserter(ids), to_id);
            return ids;
        }

        /** Get a list of all available lane numbers
         *
         * @return vector of lane numbers
         */
        id_vector lanes() const
        {
            id_set_t id_set;
            std::transform(begin(), end(), std::inserter(id_set, id_set.begin()), to_lane);
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
            for (const_iterator b = begin(); b != end(); ++b)
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
            id_set_t tile_number_set;
            populate_tile_numbers_for_lane(tile_number_set, lane);
            id_vector tile_numbers(tile_number_set.begin(), tile_number_set.end());
            return tile_numbers;
        }
        /** Get a list of all available tile numbers for the specified lane
         *
         * @note this does not clear the set!
         *
         * @param tile_number_set destination set to ensure unique tile numbers
         * @param lane lane number
         */
        void populate_tile_numbers_for_lane(id_set_t& tile_number_set, const uint_t lane) const
        {
            transform_if(begin(),
                         end(),
                         std::inserter(tile_number_set, tile_number_set.begin()),
                         lane_equals(lane),
                         to_tile);
        }
        /** Get a list of all available tile numbers for the specified lane
         *
         * @note this does not clear the set!
         *
         * @param tile_number_set destination set to ensure unique tile numbers
         * @param lane lane number
         * @param surface surface number
         * @param naming_convention tile naming convetion enum
         */
        void populate_tile_numbers_for_lane_surface(id_set_t& tile_number_set,
                                                    const uint_t lane,
                                                    const uint_t surface,
                                                    const constants::tile_naming_method naming_convention) const
        {
            transform_if(begin(),
                         end(),
                         std::inserter(tile_number_set, tile_number_set.begin()),
                         lane_surface_equals(lane, surface, naming_convention),
                         to_tile);
        }

        /** Get a list of all available tile numbers
         *
         * @return vector of tile numbers
         */
        id_vector tile_numbers() const
        {
            id_set_t tile_number_set;
            transform(begin(),
                         end(),
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
            metrics_for_lane(lane_metrics, lane);
            return lane_metrics;
        }

        /** Get a list of all available metrics for the specified lane
         *
         * @param lane_metrics destination metric array
         * @param lane lane number
         */
        void metrics_for_lane(metric_array_t& lane_metrics, const uint_t lane) const
        {
            lane_metrics.reserve(size());
            copy_if(begin(),
                    end(),
                    std::back_inserter(lane_metrics),
                    lane_equals(lane));
            metric_array_t(lane_metrics).swap(lane_metrics); // Shrink to fit
        }

        /** Get a list of all cycles listed in the metric set
         *
         * @note Returns empty array for metrics that do not have a cycle identifier
         * @return vector of cycle numbers
         */
        id_vector cycles() const
        {
            id_set_t cycle_set;
            cycles(cycle_set);
            return id_vector(cycle_set.begin(), cycle_set.end());
        }

        /** Get a list of all available metrics for the specified cycle
         *
         * @note Returns empty array for Tile Metrics, which does not contain a cycle identifier
         * @param cycle cycle number
         * @return vector of metrics that map to the given cycle
         */
        metric_array_t metrics_for_cycle(const uint_t cycle) const
        {
            return metrics_for_cycle(cycle, base_t::null());
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
            header_type::clear();
            m_id_map.clear();
            m_data.clear();
            m_version=0;
            m_data_source_exists=false;
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
        { return metric_attributes<T>::prefix(); }

        /** Get the suffix of the InterOp filename
         *
         * @return suffix
         */
        static const char *suffix()
        { return metric_attributes<T>::suffix(); }

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
            typename offset_map_t::const_iterator it = m_id_map.find(key);
            if (it == m_id_map.end())
                INTEROP_THROW( index_out_of_bounds_exception,
                        "No tile available: key: " << (key) << " map: " <<
                        (m_id_map.size()) << " == data: " <<
                        (size()));
            INTEROP_ASSERT(it->second < size());
            return m_data[it->second];
        }
        /** Get the current id offset map
         *
         * @return id offset map
         */
        offset_map_t& offset_map()
        {
            return m_id_map;
        }

    private:
        metric_array_t metrics_for_cycle(const uint_t cycle, const constants::base_cycle_t*) const
        {
            metric_array_t cycle_metrics;
            cycle_metrics.reserve(size());
            copy_if(begin(),
                    end(),
                    std::back_inserter(cycle_metrics),
                    cycle_equals(cycle));
            metric_array_t(cycle_metrics).swap(cycle_metrics); // Shrink to fit
            return cycle_metrics;
        }

        metric_array_t metrics_for_cycle(const uint_t, const void *) const
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

        void cycles(id_set_t& cycles_set) const
        {
            cycles(cycles_set, base_t::null());
        }

        void cycles(id_set_t& cycles_set, const constants::base_cycle_t*) const
        {
            std::transform(begin(), end(), std::inserter(cycles_set, cycles_set.begin()), to_cycle);
        }

        void cycles(id_set_t&, const void *) const
        {
        }

        static uint_t to_cycle(const metric_type &metric)
        {
            //TODO: ensure that this isn't called from a non-base_cycle metric set?
            return metric.cycle();
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
            lane_equals(const uint_t lane) : m_lane(lane)
            { }

            bool operator()(const metric_type &metric) const
            { return metric.lane() == m_lane; }

            const uint_t m_lane;
        };
        struct lane_surface_equals
        {
            lane_surface_equals(const uint_t lane,
                                const uint_t surface,
                                const constants::tile_naming_method naming_convention) :
                    m_lane(lane), m_surface(surface), m_naming_convention(naming_convention)
            { }

            bool operator()(const metric_type &metric) const
            { return check(metric, base_t::null());}

            bool check(const metric_type& metric, const void *)const
            {
                return metric.lane() == m_lane && metric.surface(m_naming_convention) == m_surface;
            }

            const uint_t m_lane;
            const uint_t m_surface;
            const constants::tile_naming_method m_naming_convention;
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
        /** Does the file or other source exist */
        bool m_data_source_exists;

        // TODO: remove the following
        /** Map unique identifiers to the index of the metric */
        offset_map_t m_id_map;
    };

    /** Get metric set for a given metric set */
    template<class Metric>
    struct metric_set_helper
    {
        /** Define a metric set type */
        typedef metric_set<Metric> metric_set_t;
    };
    /** Get metric set for a given metric */
    template<class Metric>
    struct metric_set_helper< metric_set<Metric> >
    {
        /** Define a metric set type */
        typedef metric_set<Metric> metric_set_t;
    };
}}}}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


