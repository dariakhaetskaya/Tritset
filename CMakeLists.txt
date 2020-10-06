cmake_minimum_required(VERSION 3.16)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES  trit.cpp trit.h test.cpp)
add_subdirectory(lib/googletest)

include_directories(lib/googletest/googletest/include)
add_executable(lab1 ${SOURCE_FILES})
target_link_libraries(lab1 gtest gtest_main)