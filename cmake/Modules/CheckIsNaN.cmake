# Ensure best isnan function is available
# Adpoted from: https://github.com/soonhokong/fp-test/blob/master/src/cmake_modules/CheckIsNaN.cmake

include(CheckCXXSourceCompiles)


CHECK_CXX_SOURCE_COMPILES(
        "#include <cmath>\nint main() { bool a = std::isnan(0e0); return 0; }\n"
        HAVE_STD_ISNAN)

if(NOT HAVE_STD_ISNAN)
    add_definitions(-DHAVE_NO_STD_ISNAN)
    return()
endif()


CHECK_CXX_SOURCE_COMPILES(
        "#include <math.h>\nint main() { bool a = isnan(0e0); return 0; }\n"
        HAVE_ISNAN)

if(HAVE_ISNAN)
    add_definitions(-DHAVE_ISNAN)
    return()
endif()

CHECK_CXX_SOURCE_COMPILES(
        "#include <math.h>\nint main() { bool a = _isnan(0e0); return 0; }\n"
        HAVE___ISNAN)

if(HAVE___ISNAN)
    add_definitions(-DHAVE___ISNAN)
    return()
endif()

CHECK_CXX_SOURCE_COMPILES(
            "# include <float.h>\nint main() { bool a = _isnan(0e0); return 0; }\n"
            HAVE_FLOAT_H_ISNAN)


if(HAVE_FLOAT_H_ISNAN)
    add_definitions(-DHAVE_FLOAT_H_ISNAN)
    return()
endif()


CHECK_CXX_SOURCE_RUNS(
        "#include <limits>\nint main() { const float NaN=std::numeric_limits<float>::quiet_NaN();return NaN!=NaN; }\n"
        IS_NAN_CHECK_WORKING)

if(NOT IS_NAN_CHECK_WORKING)
    message(FATAL_ERROR "NaN checking is not working in your current configuration")
endif()
