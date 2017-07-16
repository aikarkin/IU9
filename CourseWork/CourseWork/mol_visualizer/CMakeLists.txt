cmake_minimum_required(VERSION 3.5)
project(mol_visualizer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGLU")

set(SOURCE_FILES mol_visualizer.cpp)

set(PMOLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../pmols/include/)

add_executable(mol_visualizer ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(openbabel REQUIRED openbabel-2.0)

include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GL_INCLUDE_DIRS} ${GLU_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${openbabel_INCLUDE_DIRS} ${PMOLS_INCLUDE_DIRS})
target_link_libraries(mol_visualizer ${OPENGL_STATIC_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLFW_STATIC_LIBRARIES} ${GLUT_STATIC_LIBRARIES} ${openbabel_STATIC_LIBRARIES} pmols)
