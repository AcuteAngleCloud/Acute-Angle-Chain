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
CMAKE_COMMAND = /usr/bin/cmake.exe

# The command to remove a file.
RM = /usr/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fengwork/AcuteAngle/Acute-Angle-Chain

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fengwork/AcuteAngle/Acute-Angle-Chain/build

# Include any dependencies generated for this target.
include contracts/hello/CMakeFiles/hello.tmp.dir/depend.make

# Include the progress variables for this target.
include contracts/hello/CMakeFiles/hello.tmp.dir/progress.make

# Include the compile flags for this target's objects.
include contracts/hello/CMakeFiles/hello.tmp.dir/flags.make

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o: contracts/hello/CMakeFiles/hello.tmp.dir/flags.make
contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o: ../contracts/hello/hello.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello && /usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hello.tmp.dir/hello.cpp.o -c /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/hello/hello.cpp

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hello.tmp.dir/hello.cpp.i"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/hello/hello.cpp > CMakeFiles/hello.tmp.dir/hello.cpp.i

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hello.tmp.dir/hello.cpp.s"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/hello/hello.cpp -o CMakeFiles/hello.tmp.dir/hello.cpp.s

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.requires:

.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.requires

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.provides: contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.requires
	$(MAKE) -f contracts/hello/CMakeFiles/hello.tmp.dir/build.make contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.provides.build
.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.provides

contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.provides.build: contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o


# Object files for target hello.tmp
hello_tmp_OBJECTS = \
"CMakeFiles/hello.tmp.dir/hello.cpp.o"

# External object files for target hello.tmp
hello_tmp_EXTERNAL_OBJECTS =

contracts/hello/hello.tmp.exe: contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o
contracts/hello/hello.tmp.exe: contracts/hello/CMakeFiles/hello.tmp.dir/build.make
contracts/hello/hello.tmp.exe: contracts/hello/CMakeFiles/hello.tmp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hello.tmp.exe"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hello.tmp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contracts/hello/CMakeFiles/hello.tmp.dir/build: contracts/hello/hello.tmp.exe

.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/build

contracts/hello/CMakeFiles/hello.tmp.dir/requires: contracts/hello/CMakeFiles/hello.tmp.dir/hello.cpp.o.requires

.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/requires

contracts/hello/CMakeFiles/hello.tmp.dir/clean:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello && $(CMAKE_COMMAND) -P CMakeFiles/hello.tmp.dir/cmake_clean.cmake
.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/clean

contracts/hello/CMakeFiles/hello.tmp.dir/depend:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fengwork/AcuteAngle/Acute-Angle-Chain /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/hello /home/fengwork/AcuteAngle/Acute-Angle-Chain/build /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/hello/CMakeFiles/hello.tmp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contracts/hello/CMakeFiles/hello.tmp.dir/depend

