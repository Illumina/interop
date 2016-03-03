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

namespace illumina {
namespace interop {
namespace util {



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
    R mean_val = nan_mean<R>(beg, end, op);
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

}
}
}

