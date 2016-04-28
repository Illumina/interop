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
#define INTEROP_ENUM_DESCRIPTION(X, Y) name_type_pair_t(#X,X)
/** This macro maps an enum to a string/enum pair */
#define INTEROP_ENUM(X) name_type_pair_t(#X,X)
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_ENUM_VALUE(X, V) name_type_pair_t(#X,X)

namespace illumina { namespace interop {  namespace constants {

/** Build a mapping between an enumeration string and type */
template<typename T>
struct name_type_vector_builder;

/** Convert between a string and an enumeration type
 */
template<typename E, template<typename> class B=name_type_vector_builder>
class enumeration
{
    typedef E enum_t;
    typedef std::map<std::string, enum_t> name_type_map_t;
    typedef std::map<enum_t, std::string> type_name_map_t;
    typedef std::vector< std::pair<enum_t, std::string> > type_name_pair_vector_t;
    typedef B<enum_t> builder_t;
public:
    /** Name/value pair for enumeration */
    typedef std::vector< std::pair<std::string,enum_t> > name_type_pair_vector_t;

public:
    /** Parse string to get enumeration type
     *
     * @param name name of enumeration type
     * @return enumeration type
     */
    static enum_t parse(const std::string& name)
    {
        typename name_type_map_t::const_iterator it = name_to_type_map().find(name);
        if(it == name_to_type_map().end()) return builder_t::unknown();
        return it->second;
    }
    /** Convert enumeration type to string representation
     *
     * @param type enumeration type
     * @return name of enumeration type
     */
    static const std::string& to_string(const enum_t type)
    {
        typename type_name_map_t::const_iterator it = type_to_name_map().find(type);
        if(it == type_to_name_map().end()) throw std::runtime_error("Unexpected error finding enum"); // TODO: Create Exception
        return it->second;
    }
    /** Get string list of available enumeration types
     *
     * @return string list of available enumeration types
     */
    static std::vector<std::string> names()
    {
        std::vector<std::string> name_vec;
        name_vec.reserve(builder_t::name_type_pair_vector().size());
        for(typename name_type_pair_vector_t::const_iterator beg = builder_t::name_type_pair_vector().begin(),
                    end=builder_t::name_type_pair_vector().end();beg != end;++beg)
            name_vec.push_back(beg->first);
        return name_vec;
    }
    /** Get enum list of available enumeration types
     *
     * @return enum list of available enumeration types
     */
    static std::vector<enum_t> enums()
    {
        std::vector<enum_t> name_vec;
        name_vec.reserve(builder_t::name_type_pair_vector().size());
        for(typename name_type_pair_vector_t::const_iterator beg = builder_t::name_type_pair_vector().begin(),
                    end=builder_t::name_type_pair_vector().end();beg != end;++beg)
            name_vec.push_back(beg->second);
        return name_vec;
    }
    /** Get name/enum pair list of available enumeration types
     *
     * @return enum list of available enumeration types
     */
    static name_type_pair_vector_t pairs()
    {
        name_type_pair_vector_t name_vec;
        name_vec.reserve(builder_t::name_type_pair_vector().size());
        for(typename name_type_pair_vector_t::const_iterator beg = builder_t::name_type_pair_vector().begin(),
                    end=builder_t::name_type_pair_vector().end();beg != end;++beg)
            name_vec.push_back(*beg);
        return name_vec;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static enum_t unknown(){return builder_t::unknown();}

private:
    static const name_type_map_t& name_to_type_map()
    {
        static const name_type_map_t m_metric_type_map(builder_t::name_type_pair_vector().begin(),
                                                       builder_t::name_type_pair_vector().end());
        return m_metric_type_map;
    }
    static const type_name_map_t& type_to_name_map()
    {
        static const type_name_map_t m_metric_type_map(type_name_pair_vector().begin(),
                                                       type_name_pair_vector().end());
        return m_metric_type_map;
    }
    static const type_name_pair_vector_t& type_name_pair_vector()
    {
        static type_name_pair_vector_t tmp;
        if(!tmp.empty()) return tmp;
        tmp.reserve(builder_t::name_type_pair_vector().size());
        for(size_t i=0;i<builder_t::name_type_pair_vector().size();++i)
            tmp.push_back(std::make_pair(builder_t::name_type_pair_vector()[i].second,
                                    builder_t::name_type_pair_vector()[i].first));
        return tmp;
    }
};

/** Specialization that maps metric types to their string names
 */
template<>
class name_type_vector_builder<metric_type>
{
    typedef std::pair<std::string, metric_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
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
    typedef std::pair<std::string, tile_naming_method> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
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
    typedef std::pair<std::string, instrument_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
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
    typedef std::pair<std::string, surface_type> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
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
class name_type_vector_builder<dna_base>
{
    typedef std::pair<std::string, dna_base> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
    {
        static const name_type_pair_t name_types[] = {INTEROP_ENUM_DNA_BASE_TYPES};
        static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
        return tmp;
    }
    /** Get the unknown type
     *
     * @return unknown type
     */
    static dna_base unknown(){return constants::UnknownBase;}
};


/** Specialization that maps metric group types to their string names
 */
template<>
class name_type_vector_builder<metric_group>
{
    typedef std::pair<std::string, metric_group> name_type_pair_t;
    typedef std::vector< name_type_pair_t > name_type_vector_t;

public:
    /** Get vector that maps enumeration string names to enumeration values
     *
     * @return vector of enumeration string names and enumeration values
     */
    static const name_type_vector_t& name_type_pair_vector()
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


}}}

#undef INTEROP_ENUM_VALUE
#undef INTEROP_ENUM
#undef INTEROP_ENUM_DESCRIPTION
