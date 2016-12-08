/** Register all metrics and formats being tested
 *
 *  @file
 *  @date 11/20/16.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <set>
#include <vector>
#include <map>
#include <string>
#include "interop/constants/enums.h"
#include "interop/io/metric_stream.h"

#ifdef _MSC_VER
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

namespace illumina{ namespace interop { namespace unittest
{
    /** Track every metrics and its available binary formats */
    class format_registry
    {
        typedef std::set<int> version_set_t;
        typedef std::vector<std::string> key_vector_t;
        typedef std::map<std::string, version_set_t > format_map_t;
    public:
        /** Define an iterator to the keys identifying each metric */
        typedef key_vector_t::const_iterator const_key_iterator;

    public:
        /** Insert a metric type and format version into the tracker
         *
         * Populate actual from tests
         *
         * @param version version of the format
         */
        template<class MetricType>
        void operator()(const MetricType&, int version)
        {
            const std::string name = io::paths::interop_basename<MetricType>();
            if(m_format_map.find(name) == m_format_map.end())
            {
                m_format_map[name] = version_set_t();
                m_keys.push_back(name);
            }
            m_format_map[name].insert(version);
        }
        /** Insert a metric set into the tracker
         *
         * Populate expected using run metrics
         */
        template<class MetricSet>
        void operator()(const MetricSet&)
        {
            typedef typename MetricSet::metric_type metric_t;
            // Dyanmic phasing does not have a read or write format
            // QByLane uses the same format as QMetrics
            // Neither of these is tested
            const constants::metric_group group = static_cast<constants::metric_group>(MetricSet::TYPE);
            if(group == constants::QByLane)
                return;
            const std::string name = io::paths::interop_basename<metric_t>();
            if(m_format_map.find(name) == m_format_map.end())
            {
                m_format_map[name] = version_set_t();
                m_keys.push_back(name);
            }
            io::copy_versions<metric_t>(std::inserter(m_format_map[name], m_format_map[name].begin()));
        }
        /** Map each metric to a set of version formats */
        format_map_t m_format_map;
        /** Map a list of metrics */
        key_vector_t m_keys;
    };

    /** Create a format register for a single argument template class */
    template<template<class>class T>
    struct registry_factory
    {
        /** Get a static instance of format registery
         *
         * @return format registry
         */
        static format_registry& instance()
        {
            static format_registry _instance;
            return _instance;
        }
    };
}}}


