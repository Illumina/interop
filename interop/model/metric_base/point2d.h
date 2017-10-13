/** Point in 2D space
 *
 *
 *  @file
 *  @date 3/1/17
 *  @version 1.0
 *  @copyright GNU Public License
 */
#pragma once
#include <limits>
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Point in 2D space
     *
     * Memory:
     *  - x 4 bytes (float32)
     *  - y 4 bytes (float32)
     *  - Total 8 bytes
     */
    class point2d
    {
    public:
        /** Constructor */
        point2d() : m_x(std::numeric_limits<float>::quiet_NaN()), m_y(std::numeric_limits<float>::quiet_NaN())
        {
        }

        /**Constructor
         *
         * @param x x-coordinate
         * @param y y-coordinate
         */
        point2d(const float x, const float y) : m_x(x), m_y(y)
        {
        }

    public:
        /** Copy constructor
         *
         * @param val type that defines m_x and m_y
         */
        point2d(const point2d &val) : m_x(val.m_x), m_y(val.m_y)
        {
        }
        /** Assign type val to point2d
         *
         * @param val type that defines m_x and m_y
         * @return point2d
         */
        point2d &operator=(const point2d &val)
        {
            m_x = val.m_x;
            m_y = val.m_y;
            return *this;
        }

    public:
        /** Get x-coordinate
         *
         * @return x-coordinate
         */
        float x() const
        {
            return m_x;
        }

        /** Get y-coordinate
         *
         * @return y-coordinate
         */
        float y() const
        {
            return m_y;
        }
        /** Get size of data structure in bytes
         *
         * @return size of data structure in bytes
         */
        size_t size_in_bytes()const
        {
            return sizeof(m_x)+ sizeof(m_y);
        }

    private:
        float m_x;
        float m_y;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}
