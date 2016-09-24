cmake_minimum_required(VERSION 3.2)
project(Lab6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(HEADER_FILES include/MathVectors.h include/Light.h include/EllipticalCylinder.h include/Drawing.h)
set(SOURCE_FILES src/main.cpp src/Light.cpp src/MathVectors.cpp src/EllipticalCylinder.cpp src/Drawing.cpp)
set(RAPIDJSON_INCLUDE_DIRS /home/alex/dev/libs/rapidjson-master/include)
add_executable(Lab6 ${SOURCE_FILES} ${HEADER_FILES})

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(RapidJSON)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} src include)
target_link_libraries(Lab6 ${OPENGL_gl_LIBRARY} ${GLFW_STATIC_LIBRARIES} ${OPENGL_STATIC_LIBRARIES} ${GLUT_STATIC_LIBRARIES})
