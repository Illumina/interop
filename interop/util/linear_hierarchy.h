/** Generate a linear hierarchy of types
 *
 * This is adopted from Modern C++ Design (Loki library).
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/type_traits.h"

namespace illumina { namespace interop { namespace hierarchy
{
    /** Generate a linear hierarchy of classes */
    template<
            class TypeList,
            template<class AtomicType, class BaseType> class Base,
            class Root=null_type
    >
    class linear_hierarchy;

    /** Generate a linear hierarchy of classes
     *
     * This specialization uses the type_list to do the actual building recursively
     */
    template<
            class T1,
            class T2,
            template<class, class> class Base,
            class Root
    >
    class linear_hierarchy< type_list<T1,T2>, Base, Root > : public Base<T1, linear_hierarchy<T2, Base, Root> >
    {
    protected:
        /** Base of this class */
        typedef Base<T1, linear_hierarchy<T2, Base, Root> > base_t;
    };

    /** Generate a linear hierarchy of classes
     *
     * This specialization is the basis case in the recursion.
     */
    template<
            class T,
            template<class, class> class Base,
            class Root
    >
    class linear_hierarchy<type_list<T,null_type>, Base, Root> : public Base<T, Root>
    {
    protected:
        /** Base of this class */
        typedef Base<T, Root> base_t;
    };

    /** Generate a linear hierarchy of classes
     *
     * This specialization handles the edge case where there is no type list
     */
    template<
            template<class, class> class Base,
            class Root
    >
    class linear_hierarchy<null_type, Base, Root> : public Root
    {
    protected:
        /** Base of this class */
        typedef Root base_t;
    };


    /** Get base at specified type */
    template<
            class T,
            class TypeList,
            template<class AtomicType, class BaseType> class Base,
            class Root=null_type
    >
    struct base_at;
    /** Get base at specified type
     *
     * This specialization does the recursive search
     */
    template<
            class T,
            class T1,
            class T2,
            template<class, class> class Base,
            class Root
    >
    struct base_at< T, type_list<T1,T2>, Base, Root >
    {
        /** Base type */
        typedef typename base_at<T,T2, Base, Root>::result_t result_t;
    };
    /** Get base at specified type
     *
     * This specialization handles the found basis case
     */
    template<
            class T1,
            class T2,
            template<class, class> class Base,
            class Root
    >
    struct base_at< T1, type_list<T1,T2>, Base, Root >
    {
        /** Base type */
        typedef Base<T1, linear_hierarchy<T2, Base, Root> > result_t;
    };
    /** Get base at specified type
     *
     * This specialization handles the found basis case
     */
    template<
            class T1,
            template<class, class> class Base,
            class Root
    >
    struct base_at< T1, type_list<T1,null_type>, Base, Root >
    {
        /** Base type */
        typedef Base<T1, Root > result_t;
    };


}}}

