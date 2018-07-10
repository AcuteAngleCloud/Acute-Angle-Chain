# Install script for directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/fc/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/builtins/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/softfloat/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/chainbase/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/wasm-jit/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/utilities/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/appbase/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/chain/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/testing/cmake_install.cmake")
  include("/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/abi_generator/cmake_install.cmake")

endif()

