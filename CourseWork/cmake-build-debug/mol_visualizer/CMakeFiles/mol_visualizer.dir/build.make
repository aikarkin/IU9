# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/alex/dev/apps/clion-2016.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/alex/dev/apps/clion-2016.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alex/dev/src/cpp/CourseWork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alex/dev/src/cpp/CourseWork/cmake-build-debug

# Include any dependencies generated for this target.
include mol_visualizer/CMakeFiles/mol_visualizer.dir/depend.make

# Include the progress variables for this target.
include mol_visualizer/CMakeFiles/mol_visualizer.dir/progress.make

# Include the compile flags for this target's objects.
include mol_visualizer/CMakeFiles/mol_visualizer.dir/flags.make

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o: mol_visualizer/CMakeFiles/mol_visualizer.dir/flags.make
mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o: ../mol_visualizer/mol_visualizer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alex/dev/src/cpp/CourseWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o -c /home/alex/dev/src/cpp/CourseWork/mol_visualizer/mol_visualizer.cpp

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.i"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alex/dev/src/cpp/CourseWork/mol_visualizer/mol_visualizer.cpp > CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.i

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.s"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alex/dev/src/cpp/CourseWork/mol_visualizer/mol_visualizer.cpp -o CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.s

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.requires:

.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.requires

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.provides: mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.requires
	$(MAKE) -f mol_visualizer/CMakeFiles/mol_visualizer.dir/build.make mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.provides.build
.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.provides

mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.provides.build: mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o


# Object files for target mol_visualizer
mol_visualizer_OBJECTS = \
"CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o"

# External object files for target mol_visualizer
mol_visualizer_EXTERNAL_OBJECTS =

../bin/mol_visualizer: mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o
../bin/mol_visualizer: mol_visualizer/CMakeFiles/mol_visualizer.dir/build.make
../bin/mol_visualizer: /usr/lib/x86_64-linux-gnu/libGL.so
../bin/mol_visualizer: /usr/lib/x86_64-linux-gnu/libGLU.so
../bin/mol_visualizer: ../lib/libpmols.a
../bin/mol_visualizer: mol_visualizer/CMakeFiles/mol_visualizer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alex/dev/src/cpp/CourseWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/mol_visualizer"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mol_visualizer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mol_visualizer/CMakeFiles/mol_visualizer.dir/build: ../bin/mol_visualizer

.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/build

mol_visualizer/CMakeFiles/mol_visualizer.dir/requires: mol_visualizer/CMakeFiles/mol_visualizer.dir/mol_visualizer.cpp.o.requires

.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/requires

mol_visualizer/CMakeFiles/mol_visualizer.dir/clean:
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer && $(CMAKE_COMMAND) -P CMakeFiles/mol_visualizer.dir/cmake_clean.cmake
.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/clean

mol_visualizer/CMakeFiles/mol_visualizer.dir/depend:
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/dev/src/cpp/CourseWork /home/alex/dev/src/cpp/CourseWork/mol_visualizer /home/alex/dev/src/cpp/CourseWork/cmake-build-debug /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mol_visualizer/CMakeFiles/mol_visualizer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mol_visualizer/CMakeFiles/mol_visualizer.dir/depend

