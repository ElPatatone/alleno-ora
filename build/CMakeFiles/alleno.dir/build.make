# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/elpatatone/Documents/alleno-ora

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elpatatone/Documents/alleno-ora/build

# Include any dependencies generated for this target.
include CMakeFiles/alleno.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/alleno.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/alleno.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/alleno.dir/flags.make

CMakeFiles/alleno.dir/src/main.cpp.o: CMakeFiles/alleno.dir/flags.make
CMakeFiles/alleno.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/alleno.dir/src/main.cpp.o: CMakeFiles/alleno.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elpatatone/Documents/alleno-ora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/alleno.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alleno.dir/src/main.cpp.o -MF CMakeFiles/alleno.dir/src/main.cpp.o.d -o CMakeFiles/alleno.dir/src/main.cpp.o -c /home/elpatatone/Documents/alleno-ora/src/main.cpp

CMakeFiles/alleno.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alleno.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elpatatone/Documents/alleno-ora/src/main.cpp > CMakeFiles/alleno.dir/src/main.cpp.i

CMakeFiles/alleno.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alleno.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elpatatone/Documents/alleno-ora/src/main.cpp -o CMakeFiles/alleno.dir/src/main.cpp.s

CMakeFiles/alleno.dir/src/database.cpp.o: CMakeFiles/alleno.dir/flags.make
CMakeFiles/alleno.dir/src/database.cpp.o: ../src/database.cpp
CMakeFiles/alleno.dir/src/database.cpp.o: CMakeFiles/alleno.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elpatatone/Documents/alleno-ora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/alleno.dir/src/database.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alleno.dir/src/database.cpp.o -MF CMakeFiles/alleno.dir/src/database.cpp.o.d -o CMakeFiles/alleno.dir/src/database.cpp.o -c /home/elpatatone/Documents/alleno-ora/src/database.cpp

CMakeFiles/alleno.dir/src/database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alleno.dir/src/database.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elpatatone/Documents/alleno-ora/src/database.cpp > CMakeFiles/alleno.dir/src/database.cpp.i

CMakeFiles/alleno.dir/src/database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alleno.dir/src/database.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elpatatone/Documents/alleno-ora/src/database.cpp -o CMakeFiles/alleno.dir/src/database.cpp.s

CMakeFiles/alleno.dir/src/workout.cpp.o: CMakeFiles/alleno.dir/flags.make
CMakeFiles/alleno.dir/src/workout.cpp.o: ../src/workout.cpp
CMakeFiles/alleno.dir/src/workout.cpp.o: CMakeFiles/alleno.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elpatatone/Documents/alleno-ora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/alleno.dir/src/workout.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alleno.dir/src/workout.cpp.o -MF CMakeFiles/alleno.dir/src/workout.cpp.o.d -o CMakeFiles/alleno.dir/src/workout.cpp.o -c /home/elpatatone/Documents/alleno-ora/src/workout.cpp

CMakeFiles/alleno.dir/src/workout.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alleno.dir/src/workout.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elpatatone/Documents/alleno-ora/src/workout.cpp > CMakeFiles/alleno.dir/src/workout.cpp.i

CMakeFiles/alleno.dir/src/workout.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alleno.dir/src/workout.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elpatatone/Documents/alleno-ora/src/workout.cpp -o CMakeFiles/alleno.dir/src/workout.cpp.s

CMakeFiles/alleno.dir/src/file.cpp.o: CMakeFiles/alleno.dir/flags.make
CMakeFiles/alleno.dir/src/file.cpp.o: ../src/file.cpp
CMakeFiles/alleno.dir/src/file.cpp.o: CMakeFiles/alleno.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elpatatone/Documents/alleno-ora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/alleno.dir/src/file.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alleno.dir/src/file.cpp.o -MF CMakeFiles/alleno.dir/src/file.cpp.o.d -o CMakeFiles/alleno.dir/src/file.cpp.o -c /home/elpatatone/Documents/alleno-ora/src/file.cpp

CMakeFiles/alleno.dir/src/file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alleno.dir/src/file.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elpatatone/Documents/alleno-ora/src/file.cpp > CMakeFiles/alleno.dir/src/file.cpp.i

CMakeFiles/alleno.dir/src/file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alleno.dir/src/file.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elpatatone/Documents/alleno-ora/src/file.cpp -o CMakeFiles/alleno.dir/src/file.cpp.s

# Object files for target alleno
alleno_OBJECTS = \
"CMakeFiles/alleno.dir/src/main.cpp.o" \
"CMakeFiles/alleno.dir/src/database.cpp.o" \
"CMakeFiles/alleno.dir/src/workout.cpp.o" \
"CMakeFiles/alleno.dir/src/file.cpp.o"

# External object files for target alleno
alleno_EXTERNAL_OBJECTS =

alleno: CMakeFiles/alleno.dir/src/main.cpp.o
alleno: CMakeFiles/alleno.dir/src/database.cpp.o
alleno: CMakeFiles/alleno.dir/src/workout.cpp.o
alleno: CMakeFiles/alleno.dir/src/file.cpp.o
alleno: CMakeFiles/alleno.dir/build.make
alleno: /usr/lib/x86_64-linux-gnu/libsqlite3.so
alleno: CMakeFiles/alleno.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/elpatatone/Documents/alleno-ora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable alleno"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alleno.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/alleno.dir/build: alleno
.PHONY : CMakeFiles/alleno.dir/build

CMakeFiles/alleno.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/alleno.dir/cmake_clean.cmake
.PHONY : CMakeFiles/alleno.dir/clean

CMakeFiles/alleno.dir/depend:
	cd /home/elpatatone/Documents/alleno-ora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elpatatone/Documents/alleno-ora /home/elpatatone/Documents/alleno-ora /home/elpatatone/Documents/alleno-ora/build /home/elpatatone/Documents/alleno-ora/build /home/elpatatone/Documents/alleno-ora/build/CMakeFiles/alleno.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/alleno.dir/depend

