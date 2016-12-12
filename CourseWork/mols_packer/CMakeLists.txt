cmake_minimum_required(VERSION 3.4)
project(mols_packer)

set(SOURCE_FILES mols_packer.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(PMOLS_INCLUDE_DIRS /home/alex/dev/src/cpp/CourseWork/pmols/include/)
set(PMOLS_STATIC_LIBRARY /home/alex/dev/src/cpp/CourseWork/lib/libpmols.so)

add_executable(mols_packer ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(openbabel REQUIRED openbabel-2.0)


include_directories(${openbabel_INCLUDE_DIRS} ${PMOLS_INCLUDE_DIRS})
target_link_libraries(mols_packer ${openbabel_STATIC_LIBRARIES} ${PMOLS_STATIC_LIBRARY})

