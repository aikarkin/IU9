cmake_minimum_required(VERSION 3.4)
project(mols_packer)

set(SOURCE_FILES src/mols_packer.cpp src/conf_parser.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(PMOLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../pmols/include/ include/)

link_directories({CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_executable(mols_packer ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
pkg_search_module(openbabel REQUIRED openbabel-2.0)
<<<<<<< HEAD
=======

>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
include_directories(${openbabel_INCLUDE_DIRS} ${PMOLS_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_link_libraries(mols_packer ${openbabel_STATIC_LIBRARIES} ${Boost_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} pmols)

