/** Utility functions for building a table
 *
 * @note These functions were move here as a work around for SWIG
 *
 *  @file
 *  @date  6/13/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once


namespace illumina { namespace interop { namespace model { namespace table {

    /** Function interface for method call with single parameter
     *
     * @note This is necessary because you cannot pass anything into an function that expects no arguments, not even void
     *
     * @param obj object corresponding to the method
     * @param param1 first value to function
     * @param func pointer to member function
     * @return functor wrapper
     */
    template<class T, typename R, typename P1, typename P2>
    R call_adapter(const T& obj, P2 param1, R (T::*func )(P1)const)
    {
        return (obj.*func)(param1);
    }
    /** Function interface for method call with single dummy parameter
     *
     * @note This is necessary because you cannot pass anything into an function that expects no arguments, not even void
     *
     * @param obj object corresponding to the method
     * @param func pointer to member function
     * @return functor wrapper
     */
    template<class T, typename R, typename P1>
    R call_adapter(const T& obj, P1, R (T::*func )()const)
    {
        return (obj.*func)();
    }


}}}}
