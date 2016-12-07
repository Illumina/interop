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
#include "interop/model/run/info.h"
#include "interop/io/metric_file_stream.h"
#include "interop/util/length_of.h"

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
        /** Get name of the test generator
         *
         * @return name of metric plus version of format
         */
        static std::string name()
        {
            return std::string() + Metric::prefix()+Metric::suffix()+util::lexical_cast<std::string>(VERSION);
        }
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
    };

}}}

