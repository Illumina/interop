/** Candle stick data point
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/plot/data_point.h"

namespace illumina { namespace interop { namespace model { namespace plot {

/** Candle stick point
 */
class candle_stick_point : public data_point<float, float>
{
public:
    /** Define float vector type for outliers */
    typedef std::vector<float> float_vector_t;
public:
    /** Constructor
     *
     * @param x x-coordinate
     * @param p25 25th percentile
     * @param p50 50th percentile
     * @param p75 75th percentile
     * @param lower lower spread
     * @param upper upper spread
     * @param outliers collection of outliers
     */
    candle_stick_point(const float x=0,
                       const float p25=0,
                       const float p50=0,
                       const float p75=0,
                       const float lower=std::numeric_limits<y_type>::quiet_NaN(),
                       const float upper=std::numeric_limits<y_type>::quiet_NaN(),
                       const float_vector_t& outliers=float_vector_t()) : data_point<float, float>(x, p50),
                                                                          m_p25(p25),
                                                                          m_p75(p75),
                                                                          m_lower(lower),
                                                                          m_upper(upper),
                                                                          m_outliers(outliers)
    {}

public:
    /** Get maximum y-value
     *
     * @return maximum y-value
     */
    float max_value()const
    {
        return std::isnan(upper()) ? y() : upper();
    }
    /** Get minimum y-value
     *
     * @return minimum y-value
     */
    float min_value()const
    {
        return std::isnan(lower()) ? y() : lower();
    }
    /** Get the 25th percentile
     *
     * @return 25th percentile
     */
    float p25()const
    {
        return m_p25;
    }
    /** Get the 25th percentile
     *
     * @return 25th percentile
     */
    float p50()const
    {
        return y();
    }
    /** Get the 25th percentile
     *
     * @return 25th percentile
     */
    float p75()const
    {
        return m_p75;
    }
    /** Get the lower spread
     *
     * @return lower spread
     */
    float lower()const
    {
        return m_lower;
    }
    /** Get the upper spread
     *
     * @return upper spread
     */
    float upper()const
    {
        return m_upper;
    }
    /** Get the upper spread
     *
     * @return upper spread
     */
    const float_vector_t& outliers()const
    {
        return m_outliers;
    }

private:
    y_type m_p25;
    y_type m_p75;
    y_type m_lower;
    y_type m_upper;
    float_vector_t m_outliers;
};


}}}}