/** Encapsulate an enum and a string description
 *
 *  @file
 *  @date 12/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <string>
#include <utility>
#include "interop/constants/enums.h"


namespace illumina { namespace interop { namespace constants
{
    /** Encapsulates an enum and a string description
     */
    template<typename Enum>
    class enum_description
    {
    public:
        /** Type of the enum */
        typedef Enum enum_t;

    public:
        /** Constructor */
        enum_description() : m_value(static_cast<Enum>(constants::Unknown)){}
        /** Constructor
         *
         * @param val enum value
         * @param description enum description
         */
        enum_description(const enum_t val, const std::string& description) : m_value(val), m_description(description){}
        /** Constructor
         *
         * @param pair enum value/description pair
         */
        enum_description(const std::pair<metric_type, std::string >& pair) :
                m_value(pair.first), m_description(pair.second){}

    public:
        /** Get the value of the enum
         *
         * @return enum value
         */
        enum_t value()const
        {
            return m_value;
        }
        /** Get the description of the enum
         *
         * @return enum description
         */
        const std::string& description()const
        {
            return m_description;
        }
        /** Implicit conversion operator
         *
         * @return enum value
         */
        operator enum_t()const
        {
            return m_value;
        }

    private:
        Enum m_value;
        std::string m_description;
    };
}}}



