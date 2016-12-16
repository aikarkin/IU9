cmake_minimum_required(VERSION 3.4)

project(pmols)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(HEADER_FILES include/CellLinkedLists.h include/Molecule.h include/MolLattice.h include/MolShell.h)
set(SOURCE_FILES src/CellLinkedLists.cpp src/Molecule.cpp src/MolLattice.cpp src/MolShell.cpp)

find_package(PkgConfig REQUIRED)
pkg_search_module(openbabel REQUIRED openbabel-2.0)

add_library(pmols SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(pmols ${openbabel_STATIC_LIBRARIES})
target_include_directories(pmols 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include> 
	PRIVATE src ${openbabel_INCLUDE_DIRS})
