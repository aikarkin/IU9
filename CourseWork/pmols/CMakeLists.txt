cmake_minimum_required(VERSION 3.4)

project(pmols)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES include/cell_linked_lists.h include/mol.h include/mol_lattice.h include/mol_shell.h src/pack_optimization.cpp include/pack_optimization.h)
set(SOURCE_FILES src/cell_linked_lists.cpp src/mol.cpp src/mol_lattice.cpp src/mol_shell.cpp src/pack_optimization.cpp include/pack_optimization.h)
set(RAPIDJSON_INCLUDE_DIRS /home/alex/dev/libs/rapidjson-master/include)

find_package(PkgConfig REQUIRED)
find_package(Boost 1.58 REQUIRED)
find_package(RapidJSON)

pkg_search_module(openbabel REQUIRED openbabel-2.0)

add_library(pmols STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(pmols ${openbabel_STATIC_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${BOOST_MATH_LIBRARY})
target_include_directories(pmols 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include> 
	PRIVATE src ${openbabel_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
