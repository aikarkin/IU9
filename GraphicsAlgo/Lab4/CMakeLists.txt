cmake_minimum_required(VERSION 3.2)
project(Lab4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES lab4.h lab4.cpp)
add_executable(Lab4 ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Lab4 ${OPENGL_gl_LIBRARY} ${GLFW_STATIC_LIBRARIES} ${OPENGL_STATIC_LIBRARIES} ${GLUT_STATIC_LIBRARIES})
