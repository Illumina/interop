/** Exception utilities
 *
 * The header provides a macro to throw more informative exceptions
 *
 *  @file
 *  @date 6/25/18
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#pragma once

#if (!defined(__cplusplus) || __cplusplus < 201700L) && 0
#define INTEROP_THROW_SPEC(SPEC) throw SPEC
#else
#define INTEROP_THROW_SPEC(SPEC)
#endif

