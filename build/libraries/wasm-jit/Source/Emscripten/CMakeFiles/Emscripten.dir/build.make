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
include libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/depend.make

# Include the progress variables for this target.
include libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/flags.make

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/flags.make
libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o: ../libraries/wasm-jit/Source/Emscripten/Emscripten.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && /usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Emscripten.dir/Emscripten.cpp.o -c /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/wasm-jit/Source/Emscripten/Emscripten.cpp

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Emscripten.dir/Emscripten.cpp.i"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/wasm-jit/Source/Emscripten/Emscripten.cpp > CMakeFiles/Emscripten.dir/Emscripten.cpp.i

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Emscripten.dir/Emscripten.cpp.s"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/wasm-jit/Source/Emscripten/Emscripten.cpp -o CMakeFiles/Emscripten.dir/Emscripten.cpp.s

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.requires:

.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.requires

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.provides: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.requires
	$(MAKE) -f libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/build.make libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.provides.build
.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.provides

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.provides.build: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o


# Object files for target Emscripten
Emscripten_OBJECTS = \
"CMakeFiles/Emscripten.dir/Emscripten.cpp.o"

# External object files for target Emscripten
Emscripten_EXTERNAL_OBJECTS =

libraries/wasm-jit/Source/Emscripten/libEmscripten.a: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o
libraries/wasm-jit/Source/Emscripten/libEmscripten.a: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/build.make
libraries/wasm-jit/Source/Emscripten/libEmscripten.a: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libEmscripten.a"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && $(CMAKE_COMMAND) -P CMakeFiles/Emscripten.dir/cmake_clean_target.cmake
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Emscripten.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/build: libraries/wasm-jit/Source/Emscripten/libEmscripten.a

.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/build

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/requires: libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/Emscripten.cpp.o.requires

.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/requires

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/clean:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten && $(CMAKE_COMMAND) -P CMakeFiles/Emscripten.dir/cmake_clean.cmake
.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/clean

libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/depend:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fengwork/AcuteAngle/Acute-Angle-Chain /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/wasm-jit/Source/Emscripten /home/fengwork/AcuteAngle/Acute-Angle-Chain/build /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/wasm-jit/Source/Emscripten/CMakeFiles/Emscripten.dir/depend

