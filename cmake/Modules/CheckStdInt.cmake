# Functions to test implementation of stdint.h
#
# Functions:
#   1. check_int64_long - test whether stdint.h int64_t is long type
#
#

function(check_int64_long _flag)
    file(WRITE ${CMAKE_BINARY_DIR}/test_int64_long.cpp
            "\#include <stdint.h>
             \#include <vector>
             std::vector<int64_t>* test(){return (std::vector<long>*)0;}
             int main(){test();return 0;}
             ")
    try_compile(${_flag} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/test_int64_long.cpp)
endfunction()

