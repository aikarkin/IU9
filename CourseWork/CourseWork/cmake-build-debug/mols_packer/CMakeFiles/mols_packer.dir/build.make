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
include mols_packer/CMakeFiles/mols_packer.dir/depend.make

# Include the progress variables for this target.
include mols_packer/CMakeFiles/mols_packer.dir/progress.make

# Include the compile flags for this target's objects.
include mols_packer/CMakeFiles/mols_packer.dir/flags.make

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o: mols_packer/CMakeFiles/mols_packer.dir/flags.make
mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o: ../mols_packer/mols_packer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alex/dev/src/cpp/CourseWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mols_packer.dir/mols_packer.cpp.o -c /home/alex/dev/src/cpp/CourseWork/mols_packer/mols_packer.cpp

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mols_packer.dir/mols_packer.cpp.i"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alex/dev/src/cpp/CourseWork/mols_packer/mols_packer.cpp > CMakeFiles/mols_packer.dir/mols_packer.cpp.i

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mols_packer.dir/mols_packer.cpp.s"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alex/dev/src/cpp/CourseWork/mols_packer/mols_packer.cpp -o CMakeFiles/mols_packer.dir/mols_packer.cpp.s

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.requires:

.PHONY : mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.requires

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.provides: mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.requires
	$(MAKE) -f mols_packer/CMakeFiles/mols_packer.dir/build.make mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.provides.build
.PHONY : mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.provides

mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.provides.build: mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o


# Object files for target mols_packer
mols_packer_OBJECTS = \
"CMakeFiles/mols_packer.dir/mols_packer.cpp.o"

# External object files for target mols_packer
mols_packer_EXTERNAL_OBJECTS =

../bin/mols_packer: mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o
../bin/mols_packer: mols_packer/CMakeFiles/mols_packer.dir/build.make
../bin/mols_packer: ../mols_packer/../lib/libpmols.so
../bin/mols_packer: mols_packer/CMakeFiles/mols_packer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alex/dev/src/cpp/CourseWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/mols_packer"
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mols_packer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mols_packer/CMakeFiles/mols_packer.dir/build: ../bin/mols_packer

.PHONY : mols_packer/CMakeFiles/mols_packer.dir/build

mols_packer/CMakeFiles/mols_packer.dir/requires: mols_packer/CMakeFiles/mols_packer.dir/mols_packer.cpp.o.requires

.PHONY : mols_packer/CMakeFiles/mols_packer.dir/requires

mols_packer/CMakeFiles/mols_packer.dir/clean:
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer && $(CMAKE_COMMAND) -P CMakeFiles/mols_packer.dir/cmake_clean.cmake
.PHONY : mols_packer/CMakeFiles/mols_packer.dir/clean

mols_packer/CMakeFiles/mols_packer.dir/depend:
	cd /home/alex/dev/src/cpp/CourseWork/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/dev/src/cpp/CourseWork /home/alex/dev/src/cpp/CourseWork/mols_packer /home/alex/dev/src/cpp/CourseWork/cmake-build-debug /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer /home/alex/dev/src/cpp/CourseWork/cmake-build-debug/mols_packer/CMakeFiles/mols_packer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mols_packer/CMakeFiles/mols_packer.dir/depend

