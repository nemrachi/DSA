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
CMAKE_SOURCE_DIR = C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Zadania.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Zadania.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Zadania.dir/flags.make

CMakeFiles/Zadania.dir/DSAzadanie1.c.obj: CMakeFiles/Zadania.dir/flags.make
CMakeFiles/Zadania.dir/DSAzadanie1.c.obj: ../DSAzadanie1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Zadania.dir/DSAzadanie1.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Zadania.dir\DSAzadanie1.c.obj   -c C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\DSAzadanie1.c

CMakeFiles/Zadania.dir/DSAzadanie1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Zadania.dir/DSAzadanie1.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\DSAzadanie1.c > CMakeFiles\Zadania.dir\DSAzadanie1.c.i

CMakeFiles/Zadania.dir/DSAzadanie1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Zadania.dir/DSAzadanie1.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\DSAzadanie1.c -o CMakeFiles\Zadania.dir\DSAzadanie1.c.s

# Object files for target Zadania
Zadania_OBJECTS = \
"CMakeFiles/Zadania.dir/DSAzadanie1.c.obj"

# External object files for target Zadania
Zadania_EXTERNAL_OBJECTS =

Zadania.exe: CMakeFiles/Zadania.dir/DSAzadanie1.c.obj
Zadania.exe: CMakeFiles/Zadania.dir/build.make
Zadania.exe: CMakeFiles/Zadania.dir/linklibs.rsp
Zadania.exe: CMakeFiles/Zadania.dir/objects1.rsp
Zadania.exe: CMakeFiles/Zadania.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Zadania.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Zadania.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Zadania.dir/build: Zadania.exe

.PHONY : CMakeFiles/Zadania.dir/build

CMakeFiles/Zadania.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Zadania.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Zadania.dir/clean

CMakeFiles/Zadania.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1 C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1 C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug C:\Users\emari\CLionProjects\DSA\Zadania\Zadanie1\cmake-build-debug\CMakeFiles\Zadania.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Zadania.dir/depend
