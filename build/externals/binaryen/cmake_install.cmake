# Install script for directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/externals/binaryen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/bin/aacio-s2wasm.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aacio-s2wasm.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aacio-s2wasm.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aacio-s2wasm.exe")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/ast/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/asmjs/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/cfg/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/emscripten-optimizer/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/passes/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/support/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/externals/binaryen/src/wasm/cmake_install.cmake")

endif()

