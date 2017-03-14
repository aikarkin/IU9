cmake_minimum_required(VERSION 3.4)
project(tests)

set(SOURCE_FILES ../mols_packer/src/conf_parser.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/tests)

set(PMOLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../pmols/include/ )

link_directories({CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_executable(cll cll/src/cll.cpp ${SOURCE_FILES})
add_executable(pack_stat pack_stat/src/pack_stat.cpp ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
pkg_search_module(openbabel REQUIRED openbabel-2.0)

include_directories(${openbabel_INCLUDE_DIRS} ${PMOLS_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ../mols_packer/include )
target_link_libraries(cll ${openbabel_STATIC_LIBRARIES} ${Boost_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} pmols)
target_link_libraries(pack_stat ${openbabel_STATIC_LIBRARIES} ${Boost_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} pmols)

