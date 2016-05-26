/** Functions to calculate basic statistics over InterOp metric sets
 *
 * This file contains generic statistic algorithms that will work directory over InterOp metric sets.
 *
 * @sa src/examples/example4.cpp
 *
 *  @file
 *  @date 3/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <numeric>
#include <algorithm>
#include <cmath>
#include "interop/util/assert.h"
#include "interop/util/math.h"

namespace illumina { namespace interop { namespace util {



namespace op
{
    /** Sentinel indicating no parameter
     */
    struct parameter_none_type{};
    /** Function call with a single parameter
     */
    template<class T, typename R, typename P1=parameter_none_type>
    struct const_member_function_w
    {
        /** Constructor
         *
         * @param param1 first value to function
         * @param func pointer to member function
         */
        const_member_function_w(const P1 param1, R (T::*func )(P1)const) : m_param1(param1), m_function(func){}
        /** Perform function call
         *
         * @param val previous accumulated value
         * @param obj object with value to access
         * @return addition of both values
         */
        template<class F>
        F operator()(const F val, const T& obj)const
        {
            return val+operator()(obj);
        }
        /** Perform function call
         *
         * @param obj object with value to access
         */
        R operator()(const T& obj)const
        {
            return (obj.*m_function)(m_param1);
        }

    private:
        P1 m_param1;
        R (T::*m_function )(P1)const;
    };
    /** Function call with a single parameter
     */
    template<class T, typename R>
    struct const_member_function_w<T, R, parameter_none_type>
    {
        /** Constructor
         *
         * @param func pointer to member function
         */
        const_member_function_w(R (T::*func )()const) : m_function(func){}
        /** Perform function call
         *
         * @param val previous accumulated value
         * @param obj object with value to access
         * @return addition of both values
         */
        template<class F>
        F operator()(const F val, const T& obj)const
        {
            return val+operator()(obj);
        }
        /** Perform function call
         *
         * @param obj object with value to access
         */
        R operator()(const T& obj)const
        {
            return (obj.*m_function)();
        }

    private:
        R (T::*m_function )()const;
    };
    /**Function Interface for function call with single parameter
     *
     * @param param1 first value to function
     * @param func pointer to member function
     * @return functor wrapper
     */
    template<class T, typename R, typename P2, typename P1>
    const_member_function_w<T, R, P1> const_member_function(P2 param1, R (T::*func )(P1)const)
    {
        return const_member_function_w<T, R, P1>(param1, func);
    }
    /**Function Interface for function call with single parameter
     *
     * @param func pointer to member function
     * @return functor wrapper
     */
    template<class T, typename R>
    const_member_function_w<T, R> const_member_function(R (T::*func )()const)
    {
        return const_member_function_w<T, R>(func);
    }

    /** No operation is performed on the given value
     */
    struct operator_none
    {
        /** No operation is performed
         *
         * @param val1 source value
         * @param val2 source value
         * @return addition of both values
         */
        template<typename F, typename T>
        F operator()(const F val1, const T& val2)
        {
            return static_cast<F>(val1+val2);
        }
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

/** Collect outliers below bound for a sorted collection
 *
 * @param beg iterator to start of a sorted collection
 * @param end iterator to end of a sorted collection
 * @param bound bounding value for outliers
 * @param out output iterator
 */
template<typename I, typename O, typename F>
void outliers_lower(I beg, I end, const F bound, O out)
{
    for(;beg != end;++beg)
    {
        if (*beg < bound)
        {
            *out = *beg;
            ++out;
        }
        else break;
    }
}
/** Collect outliers above bound for a sorted collection
 *
 * @param beg iterator to start of a sorted collection
 * @param end iterator to end of a sorted collection
 * @param bound bounding value for outliers
 * @param out output iterator
 */
template<typename I, typename O, typename F>
void outliers_upper(I beg, I end, const F bound, O out)
{
    if(end==beg) return;
    for(--end;beg != end;--end)
    {
        if (*end > bound)
        {
            *out = *end;
            ++out;
        }
        else return;
    }
    if(beg == end) return;
    *out = *end;
    ++out;

}
/** Calculate the given percentile
 *
 * The percentile must be an integer in percent. For example the 99th
 * percent tile would set percentile = 99.
 *
 * @note this will change the underlying array!
 *
 * @param beg iterator to start of collection
 * @param end iterator to start of collection
 * @param percentile integer in percent
 * @return iterator to requested percentile
 */
template<typename I>
I percentile(I beg, I end, const size_t percentile)
{
    INTEROP_ASSERT(percentile > 0 && percentile <= 100);
    const size_t n = static_cast<size_t>(std::distance(beg, end));
    const size_t nth_index = static_cast<size_t>(std::ceil(percentile * n / 100.0) - 1);
    I nth_element_iterator = beg + nth_index;
    std::nth_element(beg, nth_element_iterator, end);
    return nth_element_iterator;
}
/** Calculate the given percentile
 *
 * The percentile must be an integer in percent. For example the 99th
 * percent tile would set percentile = 99.
 *
 * @note this will change the underlying array!
 *
 * @param beg iterator to start of collection
 * @param end iterator to start of collection
 * @param percentile integer in percent
 * @comp comparator between two types
 * @return iterator to requested percentile
 */
template<typename I, typename Compare>
I percentile(I beg, I end, const size_t percentile, Compare comp)
{
    INTEROP_ASSERT(percentile > 0 && percentile <= 100);
    const size_t n = static_cast<size_t>(std::distance(beg, end));
    const size_t nth_index = static_cast<size_t>(std::ceil(percentile * n / 100.0) - 1);
    I nth_element_iterator = beg + nth_index;
    std::nth_element(beg, nth_element_iterator, end, comp);
    return nth_element_iterator;
}
/** Perform linear interpolation between two sets of (x,y) points given a target x value
 *
 * @param y1 y-value for point 1
 * @param y2 y-value for point 2
 * @param x1 x-value for point 1
 * @param x2 x-value for point 2
 * @param xt target x-value
 * @return linearly interoploate y-value for given target x value
 */
template<typename F>
F interpolate_linear(const F y1, const F y2, const F x1, const F x2, const F xt)
{
    return y1 + (y2-y1) / (x2-x1) * (xt - x1);
}

/** Calculate the interoploate percentile given a sorted array
 *
 * @param beg iterator to start of sorted array
 * @param end iterator to end of sorted array
 * @param percentile target percentile [0-100]
 * @return interopated value of array corresponeding to given percentile
 */
template<typename F, typename I>
F percentile_sorted(I beg, I end, const size_t percentile)
{
    INTEROP_ASSERT(beg != end);
    INTEROP_ASSERT(percentile > 0 && percentile <= 100);
    const size_t n = static_cast<size_t>(std::distance(beg, end));
    const size_t nth_index = static_cast<size_t>(std::ceil(percentile * n / 100.0) - 1);
    if(nth_index>=(n-1)) return *(end-1);
    const F y1 = *(beg+nth_index);
    const F y2 = *(beg+nth_index+1);
    const F x1 = 100.0f * (nth_index + 0.5f) / n;
    const F x2 = 100.0f * (nth_index + 0.5f + 1) / n;
    return interpolate_linear(y1, y2, x1, x2, static_cast<float>(percentile));
}
/** Sort NaNs to the end of the collection return iterator to first NaN value
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @return iterator to last non-NaN element
 */
template<typename I, typename UnaryOp>
I remove_nan(I beg, I end, UnaryOp op)
{
    return std::stable_partition(beg, end, op::nan_check<UnaryOp>(op));
}
/** Calculate the median of the collection
 *
 * @note this will change the underlying array!
 *
 * @param beg iterator to start of collection
 * @param end iterator to start of collection
 * @return iterator to requested percentile
 */
template<typename I>
I median(I beg, I end)
{
    return percentile(beg, end, 50);
}
/** Calculate the median of the collection
 *
 * @note this will change the underlying array!
 *
 * @param beg iterator to start of collection
 * @param end iterator to start of collection
 * @comp comparator between two types
 * @return iterator to requested percentile
 */
template<typename I, typename Compare>
I median(I beg, I end, Compare comp)
{
    return percentile(beg, end, 50, comp);
}

// median using nth_element
// nan_median using nth_element

/** Estimate the mean of values in a given collection
 *
 * This function skips NaN values
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = nan_mean<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @param op function that takes one value and returns another value
 * @return mean of the input collection
 */
template<typename R, typename I, typename BinaryOp>
R nan_mean(I beg, I end, BinaryOp op)
{
    ptrdiff_t n = 0;
    R sum = 0;
    for(;beg != end;++beg)
    {
        R val = op(*beg);
        if(std::isnan(val)) continue;
        sum += val;
        ++n;
    }
    return sum/n;
}
/** Estimate the variance of values in a given collection
 *
 * This function skips NaN values
 *
 * Usage:
 *  std::vector<float> values = {0,1,2,3};
 *  double mean_val = nan_variance<double>(values.begin(), values.end());
 *
 * @param beg iterator to start of collection
 * @param end iterator to end of collection
 * @param op function that takes one value and returns another value
 * @return variance of the input collection
 */
template<typename R, typename I, typename BinaryOp>
R nan_variance(I beg, I end, BinaryOp op)
{
    ptrdiff_t n = 0;
    R sum2 = 0;
    R sum3 = 0;
    for(;beg != end;++beg)
    {
        const R val = op(*beg)-mean_val;
        if(std::isnan(val)) continue;
        sum2 += val*val;
        sum3 += val;
        ++n;
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
 * @param op function that takes one value and returns another value
 * @return mean of the input collection
 */
template<typename R, typename I, typename BinaryOp>
R mean(I beg, I end, BinaryOp op)
{
    ptrdiff_t n = std::distance(beg, end);
    if(n==0) return 0;
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
    R sum2 = 0;
    R sum3 = 0;
    for(;beg != end;++beg)
    {
        const R val = op(*beg)-mean_val;
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
    return mean<R>(beg, end, op::operator_none());
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
    return variance<R>(beg, end, op::operator_none());
}

}}}

