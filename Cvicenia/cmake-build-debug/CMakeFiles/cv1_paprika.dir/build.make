# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\emari\CLionProjects\DSA\Cvicenia

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cv1_paprika.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cv1_paprika.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cv1_paprika.dir/flags.make

CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.obj: CMakeFiles/cv1_paprika.dir/flags.make
CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.obj: ../cv1-1_paprika.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\cv1_paprika.dir\cv1-1_paprika.c.obj   -c C:\Users\emari\CLionProjects\DSA\Cvicenia\cv1-1_paprika.c

CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\emari\CLionProjects\DSA\Cvicenia\cv1-1_paprika.c > CMakeFiles\cv1_paprika.dir\cv1-1_paprika.c.i

CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\emari\CLionProjects\DSA\Cvicenia\cv1-1_paprika.c -o CMakeFiles\cv1_paprika.dir\cv1-1_paprika.c.s

# Object files for target cv1_paprika
cv1_paprika_OBJECTS = \
"CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.obj"

# External object files for target cv1_paprika
cv1_paprika_EXTERNAL_OBJECTS =

cv1_paprika.exe: CMakeFiles/cv1_paprika.dir/cv1-1_paprika.c.obj
cv1_paprika.exe: CMakeFiles/cv1_paprika.dir/build.make
cv1_paprika.exe: CMakeFiles/cv1_paprika.dir/linklibs.rsp
cv1_paprika.exe: CMakeFiles/cv1_paprika.dir/objects1.rsp
cv1_paprika.exe: CMakeFiles/cv1_paprika.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cv1_paprika.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\cv1_paprika.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cv1_paprika.dir/build: cv1_paprika.exe

.PHONY : CMakeFiles/cv1_paprika.dir/build

CMakeFiles/cv1_paprika.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\cv1_paprika.dir\cmake_clean.cmake
.PHONY : CMakeFiles/cv1_paprika.dir/clean

CMakeFiles/cv1_paprika.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\emari\CLionProjects\DSA\Cvicenia C:\Users\emari\CLionProjects\DSA\Cvicenia C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug C:\Users\emari\CLionProjects\DSA\Cvicenia\cmake-build-debug\CMakeFiles\cv1_paprika.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cv1_paprika.dir/depend
