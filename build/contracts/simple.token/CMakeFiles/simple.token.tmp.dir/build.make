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
include contracts/simple.token/CMakeFiles/simple.token.tmp.dir/depend.make

# Include the progress variables for this target.
include contracts/simple.token/CMakeFiles/simple.token.tmp.dir/progress.make

# Include the compile flags for this target's objects.
include contracts/simple.token/CMakeFiles/simple.token.tmp.dir/flags.make

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/flags.make
contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o: ../contracts/simple.token/simple.token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token && /usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o -c /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/simple.token/simple.token.cpp

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple.token.tmp.dir/simple.token.cpp.i"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/simple.token/simple.token.cpp > CMakeFiles/simple.token.tmp.dir/simple.token.cpp.i

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple.token.tmp.dir/simple.token.cpp.s"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/simple.token/simple.token.cpp -o CMakeFiles/simple.token.tmp.dir/simple.token.cpp.s

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.requires:

.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.requires

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.provides: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.requires
	$(MAKE) -f contracts/simple.token/CMakeFiles/simple.token.tmp.dir/build.make contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.provides.build
.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.provides

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.provides.build: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o


# Object files for target simple.token.tmp
simple_token_tmp_OBJECTS = \
"CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o"

# External object files for target simple.token.tmp
simple_token_tmp_EXTERNAL_OBJECTS =

contracts/simple.token/simple.token.tmp.exe: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o
contracts/simple.token/simple.token.tmp.exe: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/build.make
contracts/simple.token/simple.token.tmp.exe: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simple.token.tmp.exe"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple.token.tmp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contracts/simple.token/CMakeFiles/simple.token.tmp.dir/build: contracts/simple.token/simple.token.tmp.exe

.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/build

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/requires: contracts/simple.token/CMakeFiles/simple.token.tmp.dir/simple.token.cpp.o.requires

.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/requires

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/clean:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token && $(CMAKE_COMMAND) -P CMakeFiles/simple.token.tmp.dir/cmake_clean.cmake
.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/clean

contracts/simple.token/CMakeFiles/simple.token.tmp.dir/depend:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fengwork/AcuteAngle/Acute-Angle-Chain /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/simple.token /home/fengwork/AcuteAngle/Acute-Angle-Chain/build /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/simple.token/CMakeFiles/simple.token.tmp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contracts/simple.token/CMakeFiles/simple.token.tmp.dir/depend

