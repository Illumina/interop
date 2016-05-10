/** Options to filter metrics
 *
 *  @file
 *  @date 4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include<map>
#include "interop/constants/enums.h"
#include "interop/util/length_of.h"

/** This macro maps an enum description to a string/enum pair */
#define INTEROP_ENUM_DESCRIPTION(X, Y, Z) name_type_pair_t(#X,X)
/** This macro maps an enum to a string/enum pair */
#define INTEROP_ENUM(X) name_type_pair_t(#X,X)
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_ENUM_VALUE(X, V) name_type_pair_t(#X,X)

namespace illumina { namespace interop {  namespace constants {

/** Build a mapping between an enumeration string and type */
template<typename T>
class name_type_vector_builder;

/** Convert between a string and an enumeration type
 */
template<typename E, template<typename> class B=name_type_vector_builder>
class enumeration
{
    typedef E enum_t;
    typedef B<enum_t> builder_t;
    typedef typename builder_t::key_t key_t;
    typedef std::map<key_t, enum_t> key_type_map_t;
    typedef std::map<enum_t, key_t> type_key_map_t;
    typedef std::vector< std::pair<enum_t, key_t> > type_key_pair_vector_t;
public:
    /** Name/value pair for enumeration */
    typedef std::vector< std::pair<key_t,enum_t> > key_type_pair_vector_t;

public:
    /** Parse string to get enumeration type
     *
     * @param key key of enumeration type
     * @return enumeration type
     */
    static enum_t parse(const key_t& key)
    {
        typename key_type_map_t::const_iterator it = key_to_type_map().find(key);
        if(it == key_to_type_map().end()) return builder_t::unknown();
        return it->second;
    }
    /** Convert enumeration type to string representation
     *
     * @param type enumeration type
     * @return key of enumeration type
     */
    static const key_t& to_key(const enum_t type)
    {
        typename type_key_map_t::const_iterator it = type_to_key_map().find(type);
        if(it == type_to_key_map().end()) throw std::logic_error("Unexpected error finding enum type"); // TODO: Create Exception
        return it->second;
    }
    /** Get string list of available enumeration types
     *
     * @return string list of available enumeration types
     */
    static std::vector<key_t> keys()
    {
        std::vector<key_t> key_vec;
        key_vec.reserve(builder_t::key_type_pair_vector().size());
        for(typename key_type_pair_vector_t::const_iterator beg = builder_t::key_type_pair_vector().begin(),
                    end=builder_t::key_type_pair_vector().end();beg != end;++beg)
            key_vec.push_back(beg->first);
        return key_vec;
    }
    /** Get enum list of available enumeration types
     *
     * @return enum list of available enumeration types
     */
    static std::vector<enum_t> enums()
    {
        std::vector<enum_t> name_vec;
        name_vec.reserve(builder_t::name_type_pair_vector().size());
        for(typename key_type_pair_vector_t::const_iterator beg = builder_t::name_type_pair_vector().begin(),
                    end=builder_t::name_type_pair_vector().end();beg != end;++beg)
            name_vec.push_back(beg->second);
        return name_vec;
    }
    /** Get key/enum pair list of available enumeration types
     *
     * @return enum list of available enumeration types
     */
    static key_type_pair_vector_t pairs()
    {
        key_type_pair_vector_t name_vec;
        name_vec.reserve(builder_t::key_type_pair_vector().size());
        for(typename key_type_pair_vector_t::const_iterator beg = builder_t::key_type_pair_vector().begin(),
                    end=builder_t::key_type_pair_vector().end();beg != end;++beg)
            name_vec.push_back(*beg);
        return name_vec;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static enum_t unknown(){return builder_t::unknown();}

private:
    static const key_type_map_t& key_to_type_map()
    {
        static const key_type_map_t m_metric_type_map(builder_t::key_type_pair_vector().begin(),
                                                       builder_t::key_type_pair_vector().end());
        return m_metric_type_map;
    }
    static const type_key_map_t& type_to_key_map()
    {
        static const type_key_map_t m_metric_type_map(type_key_pair_vector().begin(),
                                                      type_key_pair_vector().end());
        return m_metric_type_map;
    }
    static const type_key_pair_vector_t& type_key_pair_vector()
    {
        static type_key_pair_vector_t tmp;
        if(!tmp.empty()) return tmp;
        tmp.reserve(builder_t::key_type_pair_vector().size());
        for(size_t i=0;i<builder_t::key_type_pair_vector().size();++i)
            tmp.push_back(std::make_pair(builder_t::key_type_pair_vector()[i].second,
                                    builder_t::key_type_pair_vector()[i].first));
        return tmp;
    }
};

/** Specialization that maps metric types to their string names
 */
template<>
class name_type_vector_builder<metric_type>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, metric_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static metric_type unknown(){return constants::UnknownMetricType;}
};

/** Specialization that maps tile naming methods to their string names
 */
template<>
class name_type_vector_builder<tile_naming_method>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, tile_naming_method> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_TILE_NAMING_METHODS};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static tile_naming_method unknown(){return constants::UnknownTileNamingMethod;}
};


/** Specialization that maps instrument types to their string names
 */
template<>
class name_type_vector_builder<instrument_type>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, instrument_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_INSTRUMENT_TYPES};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static instrument_type unknown(){return constants::UnknownInstrument;}
};



/** Specialization that maps surface types to their string names
 */
template<>
class name_type_vector_builder<surface_type>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, surface_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_SURFACE_TYPES};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static surface_type unknown(){return constants::UnknownSurface;}
};


/** Specialization that maps DNA base types to their string names
 */
template<>
class name_type_vector_builder<dna_bases>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, dna_bases> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_DNA_BASE_TYPES};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static dna_bases unknown(){return constants::UnknownBase;}
};


/** Specialization that maps DNA base types to their string names
 */
template<>
class name_type_vector_builder<plot_colors>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, plot_colors> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_PLOT_COLORS};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static plot_colors unknown(){return constants::UnknownColor;}
};

/** Specialization that maps metric group types to their string names
 */
template<>
class name_type_vector_builder<metric_group>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, metric_group> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_GROUPS};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static metric_group unknown(){return constants::UnknownMetricGroup;}
};


/** Specialization that maps metric group types to their string names
 */
template<>
class name_type_vector_builder<bar_plot_options>
{
public:
    /** Define string key type */
    typedef std::string key_t;
private:
    typedef std::pair<key_t, bar_plot_options> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& key_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_BAR_PLOT_OPTIONS};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static bar_plot_options unknown(){return constants::UnknownBarPlotOption;}
};

template<typename T>
std::string to_string(T val)
{
    return constants::enumeration<T>::to_key(val);
}

}}}

#undef INTEROP_ENUM_VALUE
#undef INTEROP_ENUM
#undef INTEROP_ENUM_DESCRIPTION
