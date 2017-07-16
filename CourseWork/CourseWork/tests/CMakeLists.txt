cmake_minimum_required(VERSION 3.4)
project(tests)

set(SOURCE_FILES tests.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(PMOLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../pmols/include/)

link_directories({CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_executable(tests ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(openbabel REQUIRED openbabel-2.0)

include_directories(${openbabel_INCLUDE_DIRS} ${PMOLS_INCLUDE_DIRS})
target_link_libraries(tests ${openbabel_STATIC_LIBRARIES} pmols)

