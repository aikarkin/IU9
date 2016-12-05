cmake_minimum_required(VERSION 3.5)
project(CourseWork)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(HEADER_FILES include/Molecule.h include/MathVectors.h src/MolShell.cpp include/MolShell.h)
set(SOURCE_FILES src/mols_packer.cpp src/MathVectors.cpp src/Molecule.cpp include/Molecule.h src/MolShell.cpp include/MolShell.h src/MolLattice.cpp include/MolLattice.h)
add_executable(CourseWork ${SOURCE_FILES} ${HEADER_FILES})

find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
# pkg_search_module(GLUT REQUIRED glut2)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(openbabel REQUIRED openbabel-2.0)

include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GL_INCLUDE_DIRS} ${GLU_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${openbabel_INCLUDE_DIRS} include/)
target_link_libraries(CourseWork ${OPENGL_gl_LIBRARY} /usr/local/lib/libGLEW.so ${GLFW_STATIC_LIBRARIES} ${OPENGL_STATIC_LIBRARIES} ${GLU_STATIC_LIBRARIES} ${GLUT_STATIC_LIBRARIES} ${openbabel_STATIC_LIBRARIES})
