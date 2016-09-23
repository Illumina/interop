/** Image Dimensions
 *
 *
 *  @file
 *  @date 11/7/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <string>
#include <vector>

namespace illumina { namespace interop { namespace model { namespace run
{
    /** Dimenions of the image
     *
     */
    class image_dimensions
    {
    public:
        /**Constructor
         *
         * @param width width of the image
         * @param height height of the image
         */
        image_dimensions(size_t width = 0, size_t height = 0) : m_width(width), m_height(height)
        {}

    public:
        /** @defgroup image_dimensions Image Dimensions
         *
         * Information describing the dimensions of the tile image
         *
         * @ingroup run_info
         * @ref illumina::interop::model::run::image_dimensions "See full class description"
         * @{
         */
        /** Get the width of the image
         *
         * @return image width
         */
        size_t width() const
        { return m_width; }

        /** Get the height of the image
         *
         * @return image height
         */
        size_t height() const
        { return m_height; }
        /** @} */
    private:
        size_t m_width;
        size_t m_height;

        friend class info;
    };

}}}}


