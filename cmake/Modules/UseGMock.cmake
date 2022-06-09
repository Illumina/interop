# A CMake Module for using Google Mock
#
# This file adds the following static library targets:
#   - gmock
#   - gest_main
#
# This file adds the Google Mock headers to the include path
#
# This enables or disables (depending on the platform) a built-in version of tr1/tuple using a definition of a
# preprocessor macro.
#
# This file setups the target to, optionally, auto-download Google Mock.
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

if(NOT GMOCK_LIBRARY)
    set(GMOCK_LIBRARY gmock)
    add_library(${GMOCK_LIBRARY} STATIC IMPORTED)
    set_property(TARGET ${GMOCK_LIBRARY} PROPERTY IMPORTED_LOCATION ${GMOCK_LIBRARY_FILE})
    if(GMOCK_TARGET)
        add_dependencies(${GMOCK_LIBRARY} ${GMOCK_TARGET})
    endif()
    if(MSVC)
        target_compile_definitions(${GMOCK_LIBRARY} INTERFACE
                PRIVATE _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
                )
    endif()
    set_target_properties(${GMOCK_LIBRARY} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()


if(NOT GMOCK_MAIN_LIBRARY)
    set(GMOCK_MAIN_LIBRARY gmock_main)
    add_library(${GMOCK_MAIN_LIBRARY} STATIC IMPORTED)
    set_property(TARGET ${GMOCK_MAIN_LIBRARY} PROPERTY IMPORTED_LOCATION ${GMOCK_MAIN_LIBRARY_FILE})
    if(GMOCK_TARGET)
        add_dependencies(${GMOCK_MAIN_LIBRARY} ${GMOCK_TARGET})
    endif()
    set_target_properties(${GMOCK_MAIN_LIBRARY} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()


include_directories(${GMOCK_INCLUDE_DIR})
add_definitions(-DGMOCK_USE_OWN_TR1_TUPLE=${USE_OWN_TR1_TUPLE})
