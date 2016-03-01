/** Convert between arbitrary types
 *
 * Type conversion can be done in many ways in C++, but there is no simple portable method to
 * convert between std::string and int and back. This library provides a limited version of
 * the Boost.Lexical_cast.
 *
 *  @file
 *
 *  @date 3/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <numeric>
#include <algorithm>

namespace illumina {
namespace interop {
namespace util {



namespace op
{
    template<class T, typename R, typename P1, R (T::*F)(P1)>
    struct operator1
    {
        operator1(const P1 index) : m_index(index){}
        float operator()(const T& metric)
        {
            return metric.percentCalledIntensity(m_index);
        }
        P1 m_index;
    };
    template<class T, typename R, typename P1>
    operator1<T, R, P1, F> opt1(P1 val, R (T::*F)(P1))
    {
        return operator1<T, R, P1, F>(val);
    };

    /** No operation is performed on the given value
     */
    struct no_op
    {
        /** No operation is performed
         *
         * @param val source value
         * @return same value
         */
        template<class T>
        const T& operator()(const T& val)
        {
            return val;
        }
    };
}

/** Estimate the mean of values in a given collection
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = mean<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @param op function that takes one value and returns another value
 * @return mean of the input collection
 */
template<typename R, typename I, typename BinaryOp>
R mean(I beg, I end, BinaryOp op)
{
    ptrdiff_t n = std::distance(beg, end);
    return std::accumulate(beg, end, R(0), op)/R(n);
}
/** Estimate the variance of values in a given collection
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = variance<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @param op function that takes one value and returns another value
 * @return variance of the input collection
 */
template<typename R, typename I, typename BinaryOp>
R variance(I beg, I end, BinaryOp op)
{
    ptrdiff_t n = std::distance(beg, end);
    R mean_val = mean<R>(beg, end, op);
    R sum2 = 0;
    R sum3 = 0;
    for(;beg != end;++beg)
    {
        const R val = op(*beg)-mean;
        sum2 += val*val;
        sum3 += val;
    }
    return (sum2 - sum3*sum3/n) / (n-1);
}


/** Estimate the mean of values in a given collection
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = mean<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @return mean of the input collection
 */
template<typename R, typename I>
R mean(I beg, I end)
{
    return mean(beg, end, op::no_op());
}/** Estimate the variance of values in a given collection
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = variance<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @return variance of the input collection
 */
template<typename R, typename I>
R variance(I beg, I end)
{
    return variance(beg, end, op::no_op());
}

}
}
}

