/** Generate a list of objects
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/type_traits.h"
#include "interop/util/linear_hierarchy.h"

namespace illumina {
namespace interop {


    /** This is a single node within the list
     *
     * A node provides the following functionality:
     *  1. It stores the object
     *  2. It provides a getter for the object
     *  3. It provides a getter for the object in the next node
     *  4. It provides a visitor to act on the object
     */
    template<class T, class Base>
    class object_list_node : public Base
    {
        typedef object_list_node<T,Base> this_t;
    protected:
        /** Next node of this hierarchy */
        typedef typename Base::base_t node_t;
    protected:
        /** Get the object in this node
         *
         * @return object
         */
        T& get_ref()
        {
            return m_object;
        }
        /** Get the object in this node
         *
         * @return object
         */
        const T& get_ref()const
        {
            return m_object;
        }
        /** Apply the given operator to the object in this node and to its parent
         *
         * @param node current node
         * @param op unary operator
         */
        template<typename UnaryOp>
        static void visit(this_t& node, UnaryOp& op)
        {
            op(node.m_object);
            node_t::visit(node, op);
        }
        /** Apply the given operator to the object in this node and to its parent
         *
         * @param node current node
         * @param op unary operator
         */
        template<typename UnaryOp>
        static void visit(this_t& node, const UnaryOp& op)
        {
            op(node.m_object);
            node_t::visit(node, op);
        }
        /** Apply the given operator to the object in this node and to its parent
         *
         * @param node current node
         * @param op unary operator
         */
        template<typename UnaryOp>
        static void visit(const this_t& node, UnaryOp& op)
        {
            op(node.m_object);
            node_t::visit(node, op);
        }
        /** Apply the given operator to the object in this node and to its parent
         *
         * @param node current node
         * @param op unary operator
         */
        template<typename UnaryOp>
        static void visit(const this_t& node, const UnaryOp& op)
        {
            op(node.m_object);
            node_t::visit(node, op);
        }

    private:
        T m_object;
    };

    /** This class serves as a recursive basis case
     */
    struct last_node
    {
        /** Base of this node */
        typedef last_node base_t;
        /** Apply the given operator to the object in this node and to its parent
         */
        template<typename UnaryOp>
        static void visit(const last_node&, const UnaryOp&){}
    };

    /** This class provides access to a list of objects
     */
    template<class TypeList>
    class object_list : protected hierarchy::linear_hierarchy<TypeList, interop::object_list_node, last_node>
    {
        typedef last_node root_type;
        typedef hierarchy::linear_hierarchy<TypeList, interop::object_list_node, root_type> parent_t;
    public:
        /** Get an object from the list of the given type
         *
         * @return object
         */
        template<class T>
        T& get()
        {
            typedef typename hierarchy::base_at<T, TypeList, interop::object_list_node, root_type>::result_t current_t;
            return current_t::get_ref();
        }
        /** Get an object from the list of the given type
         *
         * @return object
         */
        template<class T>
        const T& get()const
        {
            typedef typename hierarchy::base_at<T, TypeList, interop::object_list_node, root_type>::result_t current_t;
            return current_t::get_ref();
        }
        /** Apply the given operator to every object in the list
         *
         * @param op unary operator
         */
        template<typename UnaryOp>
        void apply(UnaryOp& op)
        {
            parent_t::visit(*this, op);
        }
        /** Apply the given operator to every object in the list
         *
         * @param op unary operator
         */
        template<typename UnaryOp>
        void apply(UnaryOp& op)const
        {
            parent_t::visit(*this, op);
        }
        /** Apply the given operator to every object in the list
         *
         * @param op unary operator
         */
        template<typename UnaryOp>
        void apply(const UnaryOp& op)
        {
            parent_t::visit(*this, op);
        }
        /** Apply the given operator to every object in the list
         *
         * @param op unary operator
         */
        template<typename UnaryOp>
        void apply(const UnaryOp& op)const
        {
            parent_t::visit(*this, op);
        }
    };

}
}

