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

# Utility rule file for test_ram_limit.

# Include the progress variables for this target.
include contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/progress.make

contracts/test_ram_limit/CMakeFiles/test_ram_limit: contracts/test_ram_limit/test_ram_limit.wast.hpp
contracts/test_ram_limit/CMakeFiles/test_ram_limit: contracts/test_ram_limit/test_ram_limit.abi.hpp
contracts/test_ram_limit/CMakeFiles/test_ram_limit: contracts/test_ram_limit/test_ram_limit.wasm


contracts/test_ram_limit/test_ram_limit.wast.hpp: contracts/test_ram_limit/test_ram_limit.wast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating test_ram_limit.wast.hpp"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && echo "const char* const test_ram_limit_wast = R\"=====(" > /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast.hpp
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && cat /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast >> /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast.hpp
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && echo ")=====\";" >> /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast.hpp

contracts/test_ram_limit/test_ram_limit.abi.hpp: contracts/test_ram_limit/test_ram_limit.abi
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating test_ram_limit.abi.hpp"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && echo "const char* const test_ram_limit_abi = R\"=====(" > /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.abi.hpp
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && cat /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.abi >> /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.abi.hpp
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && echo ")=====\";" >> /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.abi.hpp

contracts/test_ram_limit/test_ram_limit.wasm: contracts/test_ram_limit/test_ram_limit.wast.hpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating WASM test_ram_limit.wasm"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/Source/Programs/aacio-wast2wasm.exe /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wasm -n

contracts/test_ram_limit/test_ram_limit.wast: contracts/test_ram_limit/test_ram_limit.s
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating WAST test_ram_limit.wast"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/bin/aacio-s2wasm.exe -o /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/test_ram_limit.wast -s 10240 test_ram_limit.s

contracts/test_ram_limit/test_ram_limit.s: contracts/test_ram_limit/test_ram_limit.bc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating textual assembly test_ram_limit.s"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && /home/fengwork/opt/wasm/bin/llc.exe -thread-model=single -asm-verbose=false -o test_ram_limit.s test_ram_limit.bc

contracts/test_ram_limit/test_ram_limit.bc: contracts/test_ram_limit/test_ram_limit.cpp.bc
contracts/test_ram_limit/test_ram_limit.bc: contracts/libc++/libc++.bc
contracts/test_ram_limit/test_ram_limit.bc: contracts/musl/libc.bc
contracts/test_ram_limit/test_ram_limit.bc: contracts/aaciolib/aaciolib.bc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking LLVM bitcode executable test_ram_limit.bc"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && /home/fengwork/opt/wasm/bin/llvm-link.exe -only-needed -o test_ram_limit.bc test_ram_limit.cpp.bc /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/libc++/libc++.bc /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/musl/libc.bc /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/aaciolib/aaciolib.bc

contracts/test_ram_limit/test_ram_limit.cpp.bc: ../contracts/test_ram_limit/test_ram_limit.cpp
contracts/test_ram_limit/test_ram_limit.cpp.bc: ../contracts/test_ram_limit/test_ram_limit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building LLVM bitcode test_ram_limit.cpp.bc"
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && /home/fengwork/opt/wasm/bin/clang.exe -emit-llvm -O3 --std=c++14 --target=wasm32 -ffreestanding -nostdlib -nostdlibinc -DBOOST_DISABLE_ASSERTS -DBOOST_EXCEPTION_DISABLE -fno-threadsafe-statics -fno-rtti -fno-exceptions -c /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/test_ram_limit/test_ram_limit.cpp -o test_ram_limit.cpp.bc -Weverything -Wno-c++98-compat -Wno-old-style-cast -Wno-vla -Wno-vla-extension -Wno-c++98-compat-pedantic -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-packed -Wno-padded -Wno-c99-extensions -Wno-documentation-unknown-command -I /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts -I /home/fengwork/AcuteAngle/Acute-Angle-Chain/externals/magic_get/include -isystem /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/libc++/upstream/include -isystem /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/musl/upstream/include -isystem /usr/include -isystem /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/libc++/upstream/include -isystem /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/musl/upstream/include -isystem /usr/include

test_ram_limit: contracts/test_ram_limit/CMakeFiles/test_ram_limit
test_ram_limit: contracts/test_ram_limit/test_ram_limit.wast.hpp
test_ram_limit: contracts/test_ram_limit/test_ram_limit.abi.hpp
test_ram_limit: contracts/test_ram_limit/test_ram_limit.wasm
test_ram_limit: contracts/test_ram_limit/test_ram_limit.wast
test_ram_limit: contracts/test_ram_limit/test_ram_limit.s
test_ram_limit: contracts/test_ram_limit/test_ram_limit.bc
test_ram_limit: contracts/test_ram_limit/test_ram_limit.cpp.bc
test_ram_limit: contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/build.make

.PHONY : test_ram_limit

# Rule to build all files generated by this target.
contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/build: test_ram_limit

.PHONY : contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/build

contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/clean:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit && $(CMAKE_COMMAND) -P CMakeFiles/test_ram_limit.dir/cmake_clean.cmake
.PHONY : contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/clean

contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/depend:
	cd /home/fengwork/AcuteAngle/Acute-Angle-Chain/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fengwork/AcuteAngle/Acute-Angle-Chain /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/test_ram_limit /home/fengwork/AcuteAngle/Acute-Angle-Chain/build /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contracts/test_ram_limit/CMakeFiles/test_ram_limit.dir/depend

