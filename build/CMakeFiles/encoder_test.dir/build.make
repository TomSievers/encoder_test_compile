# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\tom\workspaces\encoder_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\tom\workspaces\encoder_test\build

# Include any dependencies generated for this target.
include CMakeFiles/encoder_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/encoder_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/encoder_test.dir/flags.make

CMakeFiles/encoder_test.dir/src/main.cpp.obj: CMakeFiles/encoder_test.dir/flags.make
CMakeFiles/encoder_test.dir/src/main.cpp.obj: CMakeFiles/encoder_test.dir/includes_CXX.rsp
CMakeFiles/encoder_test.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\tom\workspaces\encoder_test\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/encoder_test.dir/src/main.cpp.obj"
	C:\mingw64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\encoder_test.dir\src\main.cpp.obj -c C:\Users\tom\workspaces\encoder_test\src\main.cpp

CMakeFiles/encoder_test.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/encoder_test.dir/src/main.cpp.i"
	C:\mingw64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\tom\workspaces\encoder_test\src\main.cpp > CMakeFiles\encoder_test.dir\src\main.cpp.i

CMakeFiles/encoder_test.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/encoder_test.dir/src/main.cpp.s"
	C:\mingw64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\tom\workspaces\encoder_test\src\main.cpp -o CMakeFiles\encoder_test.dir\src\main.cpp.s

CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj: CMakeFiles/encoder_test.dir/flags.make
CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj: CMakeFiles/encoder_test.dir/includes_CXX.rsp
CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj: ../src/Encoder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\tom\workspaces\encoder_test\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj"
	C:\mingw64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\encoder_test.dir\src\Encoder.cpp.obj -c C:\Users\tom\workspaces\encoder_test\src\Encoder.cpp

CMakeFiles/encoder_test.dir/src/Encoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/encoder_test.dir/src/Encoder.cpp.i"
	C:\mingw64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\tom\workspaces\encoder_test\src\Encoder.cpp > CMakeFiles\encoder_test.dir\src\Encoder.cpp.i

CMakeFiles/encoder_test.dir/src/Encoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/encoder_test.dir/src/Encoder.cpp.s"
	C:\mingw64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\tom\workspaces\encoder_test\src\Encoder.cpp -o CMakeFiles\encoder_test.dir\src\Encoder.cpp.s

# Object files for target encoder_test
encoder_test_OBJECTS = \
"CMakeFiles/encoder_test.dir/src/main.cpp.obj" \
"CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj"

# External object files for target encoder_test
encoder_test_EXTERNAL_OBJECTS =

encoder_test.exe: CMakeFiles/encoder_test.dir/src/main.cpp.obj
encoder_test.exe: CMakeFiles/encoder_test.dir/src/Encoder.cpp.obj
encoder_test.exe: CMakeFiles/encoder_test.dir/build.make
encoder_test.exe: CMakeFiles/encoder_test.dir/linklibs.rsp
encoder_test.exe: CMakeFiles/encoder_test.dir/objects1.rsp
encoder_test.exe: CMakeFiles/encoder_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\tom\workspaces\encoder_test\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable encoder_test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\encoder_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/encoder_test.dir/build: encoder_test.exe

.PHONY : CMakeFiles/encoder_test.dir/build

CMakeFiles/encoder_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\encoder_test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/encoder_test.dir/clean

CMakeFiles/encoder_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\tom\workspaces\encoder_test C:\Users\tom\workspaces\encoder_test C:\Users\tom\workspaces\encoder_test\build C:\Users\tom\workspaces\encoder_test\build C:\Users\tom\workspaces\encoder_test\build\CMakeFiles\encoder_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/encoder_test.dir/depend

