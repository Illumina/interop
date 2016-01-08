# Used by the UseCSharp.cmake to copy a list of files to a destination directory
#
# This is called by add_custom_command as follows:
#   COMMAND ${CMAKE_COMMAND} -DFILES_TO_COPY="${sources}" -DDESTINATION_DIR=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/cmake/CopyListOfFiles.cmake
#
# It takes two input variables:
#   - FILES_TO_COPY - list of files to copy (may have regular expressions supported by glob)
#   - DESTINATION_DIR - the destination directory path
#
#

set(FILES_TO_COPY_EX)
foreach(filename in ${FILES_TO_COPY})
    file(GLOB GLOB_FILES ${filename})
    list(APPEND FILES_TO_COPY_EX ${GLOB_FILES})
endforeach()

list(LENGTH ${FILES_TO_COPY} FILES_LEN)
if(${FILES_LEN} EQUAL 1)
    get_filename_component(FILE_EXPR ${FILES_TO_COPY} NAME)
    file(REMOVE ${DESTINATION_DIR}/${FILE_EXPR})
endif()
file(COPY ${FILES_TO_COPY_EX} DESTINATION ${DESTINATION_DIR})

