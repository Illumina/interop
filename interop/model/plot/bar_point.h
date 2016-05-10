/** Bar data point
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

/** Bar data point
 */
class bar_point : public data_point<float, float>
{
public:
    /** Constructor
     *
     * @param x x-coordinate
     * @param height bar height
     * @param width bar width
     */
    bar_point(const float x=0,
              const float height=0,
              const float width=0) :
            data_point<float, float>(x, height), m_width(width)
    {}

public:
    /** Add a coordinate to the point
     *
     * @param x x-coordinate
     * @param height height of the bar
     * @param width width of the bar
     */
    void set(const x_type x, const y_type height, const float width=1)
    {
        data_point<float, float>::set(x, height);
        m_width = width;
    }
    /** Get width of the bar
     *
     * @return bar width
     */
    float width()const
    {
        return m_width;
    }

private:
    float m_width;
};


}}}}
