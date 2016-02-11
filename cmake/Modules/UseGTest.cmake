# A CMake Module for using Google Test
#
# This file adds the following static library targets:
#   - gtest
#   - gest_main
#
# This file adds the Google Test headers to the include path
#
# This enables or disables (depending on the platform) a built-in version of tr1/tuple using a definition of a
# preprocessor macro.
#
# This file setups the target to, optionally, auto-download Google Test.
#
#

if(MSVC)
    set(USE_OWN_TR1_TUPLE 0)
else()
    set(USE_OWN_TR1_TUPLE 1)
endif()

if(MSVC)
    set(USE_OWN_TR1_TUPLE 0)
else()
    set(USE_OWN_TR1_TUPLE 1)
endif()

if(NOT GTEST_LIBRARY)
    set(GTEST_LIBRARY gtest)
    add_library(${GTEST_LIBRARY} STATIC IMPORTED)
    set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION ${GTEST_LIBRARY_FILE})
    if(GTEST_TARGET)
        add_dependencies(${GTEST_LIBRARY} ${GTEST_TARGET})
    endif()
    set_target_properties(${GTEST_LIBRARY} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()


if(NOT GTEST_MAIN_LIBRARY)
    set(GTEST_MAIN_LIBRARY gtest_main)
    add_library(${GTEST_MAIN_LIBRARY} STATIC IMPORTED)
    set_property(TARGET ${GTEST_MAIN_LIBRARY} PROPERTY IMPORTED_LOCATION ${GTEST_MAIN_LIBRARY_FILE})
    if(GTEST_TARGET)
        add_dependencies(${GTEST_MAIN_LIBRARY} ${GTEST_TARGET})
    endif()
    set_target_properties(${GTEST_MAIN_LIBRARY} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()


include_directories(${GTEST_INCLUDE_DIR})
add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=${USE_OWN_TR1_TUPLE})
