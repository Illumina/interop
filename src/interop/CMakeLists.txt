
find_package(RapidXML REQUIRED)
include_directories(${RAPIDXML_INCLUDE_DIR})

set(SRCS
        model/metrics/corrected_intensity_metric.cpp
        model/metrics/tile_metric.cpp
        model/metrics/error_metric.cpp
        model/metrics/extraction_metric.cpp
        model/metrics/image_metric.cpp
        model/metrics/q_metric.cpp
        model/metrics/index_metric.cpp
        ../../interop/model/metric_sets/error_metric_set.h
        ../../interop/model/metric_sets/corrected_intensity_metric_set.h
        ../../interop/model/metrics/tile_metric.h
        ../../interop/model/metrics/corrected_intensity_metric.h
        ../../interop/io/layout/base_metric.h
        ../../interop/model/metric_base/metric_set.h
        ../../interop/model/metric_base/base_metric.h
        ../../interop/model/metric_base/base_cycle_metric.h
        ../../interop/model/metric_base/base_read_metric.h
        ../../interop/util/filesystem.h
        ../../interop/util/unique_ptr.h
        ../../interop/util/lexical_cast.h
        ../../interop/io/stream_exceptions.h
        ../../interop/io/format/abstract_metric_format.h
        ../../interop/io/format/metric_format.h
        ../../interop/io/format/metric_format_factory.h
        ../../interop/io/metric_stream.h
        ../../interop/io/format/generic_layout.h
        ../../interop/model/metrics/error_metric.h
        ../../interop/model/metrics/extraction_metric.h
        ../../interop/model/metrics/image_metric.h
        ../../interop/model/metrics/q_metric.h
        ../../interop/io/format/stream_util.h
        ../../interop/util/static_assert.h
        ../../interop/util/cstdint.h
        ../../interop/util/pstdint.h
        ../../interop/constants/enums.h
        ../../interop/model/metric_sets/extraction_metric_set.h
        ../../interop/model/metric_sets/image_metric_set.h
        ../../interop/model/metric_sets/q_metric_set.h
        ../../interop/model/metric_sets/tile_metric_set.h
        ../../interop/io/metric_file_stream.h
        ../../interop/model/metric_base/metric_exceptions.h
        ../../interop/util/assert.h
        ../../interop/io/format/map_io.h
        ../../interop/util/length_of.h
        ../../interop/model/metrics/index_metric.h
        ../../interop/model/metric_sets/index_metric_set.h
        ../../interop/util/xml_parser.h
        ../../interop/util/xml_exceptions.h
        )

include(CheckFunctionExists)
check_function_exists("strptime" HAVE_STRPTIME)
if(HAVE_STRPTIME)
    add_definitions(-DHAVE_STRPTIME)
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    #set(SRCS ${SRCS} port/strptime.c)
endif()

add_library(${INTEROP_LIB} ${LIBRARY_TYPE} ${SRCS})
add_library(${INTEROP_DL_LIB} ${LIBRARY_TYPE} ${SRCS})
add_dependencies(${INTEROP_LIB} version ${RAPIDXML_TARGET})
add_dependencies(${INTEROP_DL_LIB} version ${RAPIDXML_TARGET})

set_target_properties(${INTEROP_DL_LIB} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
if(CMAKE_COMPILER_IS_GNUCC AND NOT MINGW)
    set_target_properties(${INTEROP_DL_LIB} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()


find_package(Git REQUIRED)
add_custom_target(Deploy
        COMMAND ${GIT_EXECUTABLE} pull
        COMMAND ${GIT_EXECUTABLE} rm -r ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/include/interop
        COMMAND ${GIT_EXECUTABLE} rm -r ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/lib64
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${INTEROP_LIB}> ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/lib64/$<TARGET_FILE_NAME:${INTEROP_LIB}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/interop ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/include/interop
        COMMAND ${GIT_EXECUTABLE} add ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/include/interop
        COMMAND ${GIT_EXECUTABLE} add ${CMAKE_SOURCE_DIR}/../ipadep/${CMAKE_SYSTEM_NAME}/interop/lib64
        COMMAND ${GIT_EXECUTABLE} commit -am "Updated ${CMAKE_SYSTEM_NAME} library"
        COMMAND ${GIT_EXECUTABLE} push origin master
        DEPENDS ${INTEROP_LIB}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../ipadep/
        COMMENT "Deploy InterOp library" )

install(TARGETS ${INTEROP_LIB}
        LIBRARY DESTINATION lib64
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib64
)