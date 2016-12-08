/** Traits for types used in C++ templates
 *
 * This is adopted from Modern C++ Design (Loki library).
 *
 *  @file
 *  @date 2/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

namespace illumina { namespace interop
{

    /** Place holder for */
    struct null_type{};

    /** Linked list of types */
    template<class T, class U>
    struct type_list
    {
        /** Head of the list */
        typedef T head_t;
        /** Next type */
        typedef U tail_t;
    };
    /** Make a type list */
    template<
            typename T1=null_type, typename T2=null_type, typename T3=null_type, typename T4=null_type,
            typename T5=null_type, typename T6=null_type, typename T7=null_type, typename T8=null_type,
            typename T9=null_type, typename T10=null_type, typename T11=null_type, typename T12=null_type,
            typename T13=null_type, typename T14=null_type, typename T15=null_type, typename T16=null_type,
            typename T17=null_type, typename T18=null_type, typename T19=null_type, typename T20=null_type,
            typename T21=null_type, typename T22=null_type, typename T23=null_type, typename T24=null_type>
    struct make_type_list
    {
    private:
        typedef typename make_type_list<
                T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24
        >::result_t tail_result_t;
    public:
        /** List of types */
        typedef type_list<T1, tail_result_t > result_t;
    };
    /** Make an empty type list */
    template<>
    struct make_type_list<>
    {
        /** Empty list */
        typedef null_type result_t;
    };
    /** Transform type list */
    template<class TypeList, template<class>class Transform>
    struct transform_type_list;
    /** Transform type list
     *
     * This specialization is the recursive call
     */
    template<class T1, class T2, template<class>class Transform>
    struct transform_type_list< type_list<T1, T2>, Transform >
    {
        /** Result of transform for this specialization */
        typedef type_list< typename Transform<T1>::result_t, typename transform_type_list<T2, Transform>::result_t> result_t;
    };
    /** Transform type list
     *
     * This specialization is the recursive basis case
     */
    template<class T1, template<class>class Transform>
    struct transform_type_list< type_list<T1, null_type>, Transform >
    {
        /** Result of transform for this specialization */
        typedef type_list< typename Transform<T1>::result_t, null_type> result_t;
    };
    /** Transform type list
     *
     * This specialization is the recursive edge case
     */
    template<template<class>class Transform>
    struct transform_type_list< null_type, Transform >
    {
        /** Result of transform for this specialization: null_type */
        typedef null_type result_t;
    };

    /** Integral constant of type true */
    struct true_type
    {
        enum{
            /** True value */
            value = true
        };
    };
    /** Integral constant of type false */
    struct false_type
    {
        enum{
            /** False value */
            value = false
        };
    };
    /** Test if type is a raw pointer */
    template<typename T> struct is_pointer : false_type {};
    /** Test if type is a raw pointer */
    template<typename T> struct is_pointer<T*> : true_type {};
    /** Test if type has a const qualifier */
    template<typename T> struct is_const : false_type {};
    /** Test if type has a const qualifier */
    template<typename T> struct is_const<const T> : true_type {};
    /** Select type based on boolean condition */
    template<bool B, typename T, typename F> struct conditional { /** Selected type */ typedef T type; };
    /** Select type based on boolean condition */
    template<typename T, typename F> struct conditional<false, T, F> { /** Selected type */typedef F type; };
    /** Test if two types are the same */
    template<typename T1, typename T2>
    struct is_same : false_type {};
    /** Test if two types are the same */
    template<typename T>
    struct is_same<T,T>: true_type {};

    /** Define a constant of an arbitray type
     */
    template<typename T, T v>
    struct constant_type
    {
        /** Define the value type */
        typedef T value_t;
        /** Define the pointer type */
        typedef constant_type<T, v>* pointer_t;
        /** Get the value of the constant type
         *
         * @return value
         */
        static value_t value(){ return v; }
        /** Get the value of the constant type
         *
         * @return value
         */
        operator value_t()const { return v; }/*noexcept*/
        /** Get the value of the constant type
         *
         * @return value
         */
        value_t operator()()const { return v; }/*noexcept*/
        /** Null pointer type
         *
         * @return null pointer
         */
        static pointer_t null(){return 0;}
    };
    /** Define a constant of an int type
     */
    template<int v>
    struct int_constant_type : public constant_type<int, v>{};
}}

