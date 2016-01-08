#
# A CMake Module for finding and using the RapidXML C++ XML Parser
#
# The following variables may be used to set the path of the RapidXML C++ XML Parser
#   - RAPIDXML_ROOT: a cmake variable
#   - RAPIDXML_DIR: an environment variable
#
# If RapidXML is not found, an attempt will be made to auto-download and make avaialble.
#
# The following variables are set:
#   - RAPIDXML_INCLUDE_DIR - include path for RapidXML C++ XML Parser
#   - RAPIDXML_FOUND - set to ON if RapidXML is found
#

find_package(PkgConfig)
pkg_check_modules(PC_RAPIDXML QUIET rapidxml)

set(RAPIDXML_ROOT "" CACHE PATH "Set the location of the RapidXML library and header")

find_path(RAPIDXML_INCLUDE_DIR
        NAMES rapidxml.hpp
        HINTS ${PC_RAPIDXML_INCLUDEDIR} ${PC_RAPIDXML_INCLUDE_DIRS} ${RAPIDXML_ROOT} $ENV{RAPIDXML_DIR}
        PATH_SUFFIXES . include
        )

if(NOT RAPIDXML_INCLUDE_DIR)
    include(ExternalProject)
    ExternalProject_Add(
            RapidXML
            URL http://downloads.sourceforge.net/project/rapidxml/rapidxml/rapidxml%201.13/rapidxml-1.13.zip
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
    )
    ExternalProject_Get_Property(RapidXML SOURCE_DIR)
    set(RAPIDXML_INCLUDE_DIR ${SOURCE_DIR})
    set(RAPIDXML_TARGET RapidXML)
else()
    set(RAPIDXML_TARGET "")
endif()

set(RAPIDXML_LIBRARIES ${RAPIDXML_INCLUDE_DIR} )
set(RAPIDXML_LIBRARY ${RAPIDXML_INCLUDE_DIR} )
set(RAPIDXML_INCLUDE_DIRS ${RAPIDXML_INCLUDE_DIR} )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidXML DEFAULT_MSG RAPIDXML_INCLUDE_DIR)

mark_as_advanced(RAPIDXML_LIBRARIES RAPIDXML_LIBRARY RAPIDXML_INCLUDE_DIRS)



