/** Options to filter metrics
 *
 *  @file
 *  @date 4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <map>
#include "interop/util/constant_mapping.h"
#include "interop/constants/enums.h"
#include "interop/util/length_of.h"

/** This macro maps an enum to a string/enum pair */
#define INTEROP_TUPLE1(X) name_type_pair_t(#X,X)
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_TUPLE2(X, IGNORED_1) name_type_pair_t(#X,X)
/** This macro maps an enum description to a string/enum pair */
#define INTEROP_TUPLE3(X, IGNORED_1, IGNORED_2) name_type_pair_t(#X,X)
/** This macro maps an enum description to a string/enum pair */
#define INTEROP_TUPLE4(X, IGNORED_1, IGNORED_2, IGNORED_3) name_type_pair_t(#X,X)

namespace illumina { namespace interop {  namespace constants {
    /** Template class declaration to map enum types to string representations
     *
     * @note only the specializations do something
     */
    template<typename Enum>
    class enumeration_string_mapping;

    template<typename Enum>
    std::string to_string(Enum value)
    {
        typedef util::constant_mapping<Enum, std::string> mapping_t;
        typedef enumeration_string_mapping<Enum> enum_map_t;
        return enum_map_t::template setup<const mapping_t&>(mapping_t::rmapping).get(value, std::string("Unknown"));
    }

    template<typename Enum>
    Enum parse(const std::string& name)
    {
        typedef util::constant_mapping<std::string, Enum> mapping_t;
        typedef enumeration_string_mapping<Enum> enum_map_t;
        return enum_map_t::template setup<const mapping_t&>(mapping_t::fmapping).get(name, static_cast<Enum>(Unknown));
    }
    namespace detail
    {
        /** Helper functor to fill an enum vector */
        template<typename Enum>
        struct fill_enum_vector
        {
            /** Constructor
             *
             * @param vec1 vector to fill
             */
            fill_enum_vector(std::vector<Enum> &vec1) : m_vec(vec1) { }
            /** Fill the vector
             *
             * @param vals string/enum pair array
             * @param n number of elements in array
             */
            void operator()(const std::pair<std::string, Enum> *vals, const size_t n) const
            {
                m_vec.resize(n);
                for (size_t i = 0; i < n; ++i)
                    m_vec[i] = vals[i].second;
            }
        private:
            std::vector<Enum> &m_vec;
        };
    }
    /** Fill given enum vector with all available enums
     *
     * @param vec enum vector
     */
    template<typename Enum>
    void list_enums(std::vector<Enum>& vec)
    {
        typedef enumeration_string_mapping<Enum> enum_map_t;
        enum_map_t::template setup<void>(detail::fill_enum_vector<Enum>(vec));
    }

    /** Specialization that maps metric_type to a string */
    template<>
    class enumeration_string_mapping<metric_type>
    {
        typedef std::pair<std::string, metric_type> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
            return func(name_types, util::length_of(name_types));
        }
    };

    /** Specialization that maps tile_naming_method to a string */
    template<>
    class enumeration_string_mapping<tile_naming_method>
    {
        typedef std::pair<std::string, tile_naming_method> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_TILE_NAMING_METHODS};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps surface_type to a string */
    template<>
    class enumeration_string_mapping<surface_type>
    {
        typedef std::pair<std::string, surface_type> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_SURFACE_TYPES};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps dna_bases to a string */
    template<>
    class enumeration_string_mapping<dna_bases>
    {
        typedef std::pair<std::string, dna_bases> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_DNA_BASE_TYPES};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps instrument_type to a string */
    template<>
    class enumeration_string_mapping<instrument_type>
    {
        typedef std::pair<std::string, instrument_type> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_INSTRUMENT_TYPES};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps plot_colors to a string */
    template<>
    class enumeration_string_mapping<plot_colors>
    {
        typedef std::pair<std::string, plot_colors> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_PLOT_COLORS};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps metric_group to a string */
    template<>
    class enumeration_string_mapping<metric_group>
    {
        typedef std::pair<std::string, metric_group> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_GROUPS};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps metric_data to a string */
    template<>
    class enumeration_string_mapping<metric_data>
    {
        typedef std::pair<std::string, metric_data> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_DATA_TYPES};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps bar_plot_options to a string */
    template<>
    class enumeration_string_mapping<bar_plot_options>
    {
        typedef std::pair<std::string, bar_plot_options> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_BAR_PLOT_OPTIONS};
            return func(name_types, util::length_of(name_types));
        }
    };
    /** Specialization that maps bar_plot_options to a string */
    template<>
    class enumeration_string_mapping<metric_feature_type>
    {
        typedef std::pair<std::string, metric_feature_type> name_type_pair_t;
    public:
        /** Pass an array of string, enum pairs and its length to the given function
         *
         * @param func pointer to a function that takes an array of string/enum pairs as a parameter
         * @return the value returned by the given function (in the case of `parse` and `to_string` the return value is a singleton)
         */
        template<typename R, typename F>
        static R setup(F func)
        {
            static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_FEATURE_TYPE};
            return func(name_types, util::length_of(name_types));
        }
    };

}}}

#undef INTEROP_TUPLE1
#undef INTEROP_TUPLE2
#undef INTEROP_TUPLE3
#undef INTEROP_TUPLE4
