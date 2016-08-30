/** Utility class for setting up a basic data fixture
 *
 *
 *  @file
 *  @date 3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <vector>
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/metric_file_stream.h"

namespace illumina{ namespace interop { namespace unittest {

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

    /** Generic metric set structure
     */
    template<class Metric, int Version>
    class metric_test
    {
    public:
        enum Checks
        {
            /** Version of the format */
            VERSION=Version,
            /** Check record size */
            disable_check_record_size = false,
            /** Check the size of the expected written binary data */
            disable_binary_data_size = false,
            /** Check the expected written binary data */
            disable_binary_data = false
        };
    public:
        /** Metric type */
        typedef Metric metric_t;
        /** Metric set type */
        typedef model::metric_base::metric_set<Metric> metric_set_t;
        /** Metric set header type */
        typedef typename metric_t::header_type header_t;
        /** Constant iterator over metrics */
        typedef typename metric_set_t::const_iterator const_iterator;

    public:
        /** Convert an array to a vector
         *
         * Determines the length of the stack array automatically.
         *
         * @param vals array pointer
         * @return vector of values
         */
        template<typename T, size_t N>
        static std::vector<T> to_vector(const T (&vals)[N])
        {
            return std::vector<T>(vals, vals + N);
        }
        /** Get size of stack array
         *
         * Determines the length of the stack array automatically.
         *
         * @return size of stack array
         */
        template<typename T, size_t N>
        static size_t size_of(const T (&)[N])
        {
            return N;
        }
        /** Convert an array to a vector
         *
         * Determines the length of the stack array automatically.
         *
         * @param vals array pointer
         * @return vector of values
         */
        template<typename T, size_t N, size_t Size>
        static std::vector<T> to_vector(const sparse_value<T, Size> (&vals)[N])
        {
            std::vector<T> vec(Size, 0);
            for (size_t i = 0; i < N; i++)
                vec[vals[i].offset] = vals[i].value;
            return vec;
        }
        /** Set expected binary data
         *
         * @param tmp int array of byte values
         * @return string of byte values
         */
        template<size_t N>
        static std::string to_string(const int (&tmp)[N])
        {
            std::string binary_data="";
            for (size_t i = 0; i < N; ++i) binary_data += char(tmp[i]);
            return binary_data;
        }
    };

    /** Generic fixture base class */
    template<class Gen>
    struct metric_test_fixture : public Gen
    {
        /** Metric type */
        typedef typename Gen::metric_t metric_t;
        /** Metric set type */
        typedef typename Gen::metric_set_t metric_set_t;
        /** Constructor
         *
         */
        metric_test_fixture() : expected_metric_set(Gen::metrics(), Gen::VERSION, Gen::header()),
                expected_binary_data(Gen::binary_data())
        {
        }
        /** Read metrics from a binary buffer
         *
         * @param data binary buffer
         * @param metrics metric set
         */
        static void read_metrics(const std::string& data, metric_set_t& metrics)
        {
            try
            {
                std::istringstream fin(data);
                illumina::interop::io::read_metrics(fin, metrics);
            }
            catch (const std::exception &) { }
        }

    public:
        /** Expected metric set */
        metric_set_t expected_metric_set;
        /** Expected buffer of binary metric data */
        std::string expected_binary_data;
        /** Actual metric set */
        metric_set_t actual_metric_set;
        /** Actual buffer of binary metric data */
        std::string actual_binary_data;
    };

    /** Setup up a hardcoded metric test */
    template<class T>
    class hardcoded_fixture : public metric_test_fixture<T>
    {
        typedef metric_test_fixture<T> fixture_t;
    public:
        /** Construct hard coded test */
        hardcoded_fixture()
        {
            fixture_t::read_metrics(fixture_t::expected_binary_data,fixture_t::actual_metric_set);
            std::ostringstream fout;
            illumina::interop::io::write_metrics(fout, fixture_t::actual_metric_set, T::VERSION);
            fixture_t::actual_binary_data = fout.str();
        }
    };

    /** Sets up a write read test */
    template<class T>
    class write_read_fixture : public metric_test_fixture<T>
    {
        typedef metric_test_fixture<T> fixture_t;
    public:
        /** Construct write read test */
        write_read_fixture()
        {
            {
                std::ostringstream fout;
                illumina::interop::io::write_metrics(fout, fixture_t::expected_metric_set, T::VERSION);
                fixture_t::expected_binary_data = fout.str();
            }
            fixture_t::read_metrics(fixture_t::expected_binary_data,fixture_t::actual_metric_set);
            {
                std::ostringstream fout;
                illumina::interop::io::write_metrics(fout, fixture_t::actual_metric_set, T::VERSION);
                fixture_t::actual_binary_data = fout.str();
            }
        }
    };


}}}
