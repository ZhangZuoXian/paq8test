# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ghj/paq8test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ghj/paq8test/build

# Include any dependencies generated for this target.
include test/CMakeFiles/RedisHanler_test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/RedisHanler_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/RedisHanler_test.dir/flags.make

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o: test/CMakeFiles/RedisHanler_test.dir/flags.make
test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o: ../test/util/RedisHanler_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghj/paq8test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o"
	cd /home/ghj/paq8test/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o -c /home/ghj/paq8test/test/util/RedisHanler_test.cpp

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.i"
	cd /home/ghj/paq8test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghj/paq8test/test/util/RedisHanler_test.cpp > CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.i

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.s"
	cd /home/ghj/paq8test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghj/paq8test/test/util/RedisHanler_test.cpp -o CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.s

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.requires:

.PHONY : test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.requires

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.provides: test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/RedisHanler_test.dir/build.make test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.provides.build
.PHONY : test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.provides

test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.provides.build: test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o


# Object files for target RedisHanler_test
RedisHanler_test_OBJECTS = \
"CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o"

# External object files for target RedisHanler_test
RedisHanler_test_EXTERNAL_OBJECTS =

test/RedisHanler_test: test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o
test/RedisHanler_test: test/CMakeFiles/RedisHanler_test.dir/build.make
test/RedisHanler_test: ../lib/libpaq8_shared.so
test/RedisHanler_test: lib/libgtest.a
test/RedisHanler_test: lib/libgmock_main.a
test/RedisHanler_test: lib/libgmock.a
test/RedisHanler_test: lib/libgtest.a
test/RedisHanler_test: test/CMakeFiles/RedisHanler_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ghj/paq8test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RedisHanler_test"
	cd /home/ghj/paq8test/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RedisHanler_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/RedisHanler_test.dir/build: test/RedisHanler_test

.PHONY : test/CMakeFiles/RedisHanler_test.dir/build

test/CMakeFiles/RedisHanler_test.dir/requires: test/CMakeFiles/RedisHanler_test.dir/util/RedisHanler_test.cpp.o.requires

.PHONY : test/CMakeFiles/RedisHanler_test.dir/requires

test/CMakeFiles/RedisHanler_test.dir/clean:
	cd /home/ghj/paq8test/build/test && $(CMAKE_COMMAND) -P CMakeFiles/RedisHanler_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/RedisHanler_test.dir/clean

test/CMakeFiles/RedisHanler_test.dir/depend:
	cd /home/ghj/paq8test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ghj/paq8test /home/ghj/paq8test/test /home/ghj/paq8test/build /home/ghj/paq8test/build/test /home/ghj/paq8test/build/test/CMakeFiles/RedisHanler_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/RedisHanler_test.dir/depend

