/** Scopped timer
 *
 *  @file
 *  @date 11/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <ctime>

namespace illumina { namespace interop { namespace util
{

    class scoped_timer
    {
    public:
        scoped_timer(double& duration) : m_start(std::clock()), m_duration(duration){}
        ~scoped_timer(){m_duration = (std::clock()-m_start) / static_cast<double>(CLOCKS_PER_SEC);}
    private:
        std::clock_t m_start;
        double& m_duration;

    };

}}}
