//
// Created by dev on 10/6/15.
//

#pragma once
#include <sstream>
#include "interop/io/metric_file_stream.h"

namespace illumina{ namespace interop { namespace unittest { namespace util {
/** This template class provides all the generic routines for setting up
 * two different types of tests:
 *  1. Hard coded binary data set
 *  2. Write/read test
 */
template<class MetricSet, int Version>
struct fixture_helper
{
    /** Sparse representation
     */
    template<class T, size_t Size>
    struct sparse_value
    {
        /** Constructor
         *
         * @param o index
         * @param v value
         */
        sparse_value(size_t o, T v) : offset(o), value(v)
        { }

        /** Index */
        size_t offset;
        /** Value */
        T value;
    };

    enum
    {
        /** Version of the metric format */
        VERSION = Version,
        /** Check record size */
        disable_check_record_size = false,
        /** Check the size of the expected written binary data */
        disable_binary_data_size = false,
        /** Check the expected written binary data */
        disable_binary_data = false
    };
    /** Metric set type */
    typedef MetricSet metrics_type;
    /** Type of the metric */
    typedef typename metrics_type::metric_type metric_type;
    /** std::vector of corrected intensity metrics per tile and cycle */
    typedef typename metrics_type::metric_array_t metric_set_vector;
    /** Bidirectional iterator to const corrected_intensity_metric& **/
    typedef typename metric_set_vector::const_iterator const_iterator;
    /** Defines a metric set header type */
    typedef typename metric_type::header_type header_type;

    /** Setup a write/read test to ensure symmetry in metric reading/writing
     *
     * @param header metric set header type
     */
    void setup_write_read(const header_type &header = header_type())
    {
        expected_metric_set = metrics_type(expected_metrics, Version, header);
        expected_binary_data = write_metrics(expected_metric_set);
        read_metrics(expected_binary_data, actual_metric_set);
        actual_metrics = actual_metric_set.metrics();
        actual_binary_data = write_metrics(actual_metric_set);
    }

    /** Setup hard coded binary data test
     *
     * Test if the InterOp reader works with real data from an InterOp file
     *
     * @param buffer binary InterOp data
     * @param header metric set header type
     */
    void setup_hardcoded_binary(const std::string &buffer, const header_type &header)
    {
        expected_metric_set = metrics_type(expected_metrics, Version, header);
        expected_binary_data = buffer;
        read_metrics(expected_binary_data, actual_metric_set);
        actual_metrics = actual_metric_set.metrics();
        actual_binary_data = write_metrics(actual_metric_set);
    }

    /** Setup hard coded binary data test
     *
     * Test if the InterOp reader works with real data from an InterOp file
     *
     * @param tmp binary InterOp data
     * @param header metric set header type
     */
    template<size_t N>
    void setup_hardcoded_binary(int (&tmp)[N], const header_type &header)
    {
        setup_hardcoded_binary(convert_int_to_binary_string(tmp, N), header);
    }

    /** Convert an array to a vector
     *
     * Determines the length of the stack array automatically.
     *
     * @param vals array pointer
     * @return vector of values
     */
    template<typename T, size_t N>
    std::vector<T> to_vector(T (&vals)[N])
    {
        return std::vector<T>(vals, vals + N);
    }

    /** Convert an array to a vector
     *
     * Determines the length of the stack array automatically.
     *
     * @param vals array pointer
     * @return vector of values
     */
    template<typename T, size_t N, size_t Size>
    std::vector<T> to_vector(sparse_value<T, Size> (&vals)[N])
    {
        std::vector<T> vec(Size, 0);
        for (size_t i = 0; i < N; i++)
            vec[vals[i].offset] = vals[i].value;
        return vec;
    }

    /** Read binary metric data encoded in a string buffer
     *
     * @param buf string buffer encoding binary metric data
     * @param metric_set metric set
     */
    void read_metrics(const std::string &buf, metrics_type &metric_set)
    {
        try
        {
            std::istringstream fin(buf);
            illumina::interop::io::read_metrics(fin, metric_set);
        }
        catch (const std::exception &)
        { }
    }

    /** Write metrics to a string stream and return a binary buffer
     *
     * @param metrics metric set
     * @return binary string buffer
     */
    std::string write_metrics(const metrics_type &metrics)
    {
        return write_metrics(metrics, VERSION);
    }

    /** Write metrics to a string stream and return a binary buffer
     *
     * @param metrics metric set
     * @param version version of the format to write
     * @return binary string buffer
     */
    std::string write_metrics(const metrics_type &metrics, int version)
    {
        std::ostringstream fout;
        illumina::interop::io::write_metrics(fout, metrics, version);
        return fout.str();
    }


    /** Create a string buffer containing binary metric data for version 3 of the corrected intensity metric InterOp.
     * This data comprises three records.
     *
     * @param tmp temporary int array of binary values
     * @param n size of int array
     * @return string buffer encoding binary metric data
     */
    static std::string convert_int_to_binary_string(int tmp[], int n)
    {
        std::string data;
        for (int i = 0; i < n; i++) data += char(tmp[i]);
        return data;
    }

    /** Metric string binary data */
    std::string expected_binary_data;
    /** Metric string binary data */
    std::string actual_binary_data;
    /** Expected metric set */
    metrics_type expected_metric_set;
    /** Expected metrics */
    metric_set_vector expected_metrics;
    /** Actual metric set */
    metrics_type actual_metric_set;
    /** Actual metrics */
    metric_set_vector actual_metrics;
};

}}}}
