/** Generic interface to the RapidXML parser
 *
 *  @file
 *
 *  @date 11/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <interop/external/rapidxml.hpp>
#include <interop/external/rapidxml_utils.hpp>
#include "interop/util/xml_exceptions.h"
#include "interop/util/exception.h"

namespace illumina { namespace interop { namespace xml
{

    /** XML node pointer type */
    typedef rapidxml::xml_node<> *xml_node_ptr;
    /** XML attribute pointer type */
    typedef rapidxml::xml_attribute<> *xml_attr_ptr;

    /** Check if the xml node matches the target, and, if so, set the value
     *
     * @param p_node current node
     * @param target target string
     * @param val destination value
     * @param default_val default value if the tag is not found
     * @return true if the target was found
     */
    template<class T>
    bool set_data_with_default(xml_node_ptr p_node, const std::string &target, T &val, const T default_val)
    {
        if (p_node == 0)
        {
            val = default_val;
            return false;
        }
        if (p_node->name() == target)
        {
            val = util::lexical_cast<T>(p_node->value());
            return true;
        }
        INTEROP_THROW(missing_xml_element_exception, "Cannot find node: " << target);
    }

    /** Find the target node and set the value
     *
     * @param p_node current node
     * @param target target string
     * @param val destination value
     * @return true if the target was found
     */
    template<class T>
    void set_data_for_target(xml_node_ptr p_node, const std::string &target, T &val)
    {
        if (p_node == 0)
            INTEROP_THROW(missing_xml_element_exception, "Cannot find parent");
        p_node = p_node->first_node(target.c_str());
        if (p_node == 0)
            INTEROP_THROW(missing_xml_element_exception, "Cannot find node: " << target);
        val = util::lexical_cast<T>(p_node->value());
    }

    /** Find the target node and set the value
     *
     * @param p_node current node
     * @param target target string
     * @param val destination value
     * @param default_val default value if the tag is not found
     * @return true if the target was found
     */
    template<class T>
    void set_data_for_target(xml_node_ptr p_node, const std::string &target, T &val, const T default_val)
    {
        if (p_node == 0) INTEROP_THROW(missing_xml_element_exception, "Cannot find parent");
        p_node = p_node->first_node(target.c_str());
        if (p_node == 0) val = default_val;
        else val = util::lexical_cast<T>(p_node->value());
    }

    /** Check if the xml node matches the target, and, if so, set the value
     *
     * @param p_node current node
     * @param target target string
     * @param val destination value
     * @return true if the target was found
     */
    template<class T>
    bool set_data(xml_node_ptr p_node, const std::string &target, T &val)
    {
        if (p_node == 0) INTEROP_THROW(missing_xml_element_exception, "Cannot find node: " << target);
        if (p_node->name() == target)
        {
            val = util::lexical_cast<T>(p_node->value());
            return true;
        }
        return false;
    }

    /** Convert the value of the node to the destination type
     *
     * @param p_attr current node
     * @param val destination value
     */
    template<class T>
    void set_data(xml_node_ptr p_attr, T &val)
    {
        if (p_attr == 0) INTEROP_THROW(missing_xml_element_exception, "Cannot find node");
        val = util::lexical_cast<T>(p_attr->value());
    }

    /** Check if the xml attributes matches the target, and, if so, set the value
     *
     * @param p_attr current attribute
     * @param target target string
     * @param val destination value
     * @return true if the target was found
     */
    template<class T>
    bool set_data(xml_attr_ptr p_attr, const std::string &target, T &val)
    {
        if (p_attr == 0) INTEROP_THROW(missing_xml_element_exception, "Cannot find attribute: " << target);
        if (p_attr->name() == target)
        {
            val = util::lexical_cast<T>(p_attr->value());
            return true;
        }
        return false;
    }

    /** Convert the value of the attribute to the destination type
     *
     * @param p_attr current attribute
     * @param val destination value
     */
    template<class T>
    void set_data(xml_attr_ptr p_attr, T &val)
    {
        if (p_attr == 0)
            INTEROP_THROW(missing_xml_element_exception, "Cannot find attribute");
        val = util::lexical_cast<T>(p_attr->value());
    }

    /** Check if the xml node matches the target, and, if so, save the children to the vector
     *
     * @param p_node current node
     * @param target target string
     * @param child child node name
     * @param val destination vector
     * @return true if the target was found
     */
    template<class T>
    bool set_data(xml_node_ptr p_node, const std::string &target, const std::string &child, std::vector<T> &val)
    {
        if (p_node == 0)
            INTEROP_THROW(missing_xml_element_exception, "Cannot find node: " << target);
        if (p_node->name() == target)
        {
            val.clear();
            for (rapidxml::xml_node<> *p_name = p_node->first_node(); p_name; p_name = p_name->next_sibling())
            {
                if (p_name->name() != child) continue;
                val.push_back(util::lexical_cast<T>(p_name->value()));
            }
            return true;
        }
        return false;
    }

}}}


