# Install script for directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain

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
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libaacio_chain.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/chain/libaacio_chain.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/aacio/chain/aacio_contract.hpp;/usr/local/include/aacio/chain/abi_def.hpp;/usr/local/include/aacio/chain/abi_serializer.hpp;/usr/local/include/aacio/chain/account_object.hpp;/usr/local/include/aacio/chain/action.hpp;/usr/local/include/aacio/chain/action_receipt.hpp;/usr/local/include/aacio/chain/apply_context.hpp;/usr/local/include/aacio/chain/asset.hpp;/usr/local/include/aacio/chain/authority.hpp;/usr/local/include/aacio/chain/authority_checker.hpp;/usr/local/include/aacio/chain/authorization_manager.hpp;/usr/local/include/aacio/chain/block.hpp;/usr/local/include/aacio/chain/block_header.hpp;/usr/local/include/aacio/chain/block_header_state.hpp;/usr/local/include/aacio/chain/block_log.hpp;/usr/local/include/aacio/chain/block_state.hpp;/usr/local/include/aacio/chain/block_summary_object.hpp;/usr/local/include/aacio/chain/block_timestamp.hpp;/usr/local/include/aacio/chain/chain_config.hpp;/usr/local/include/aacio/chain/chain_id_type.hpp;/usr/local/include/aacio/chain/config.hpp;/usr/local/include/aacio/chain/contract_table_objects.hpp;/usr/local/include/aacio/chain/contract_types.hpp;/usr/local/include/aacio/chain/controller.hpp;/usr/local/include/aacio/chain/exceptions.hpp;/usr/local/include/aacio/chain/fixed_key.hpp;/usr/local/include/aacio/chain/fork_database.hpp;/usr/local/include/aacio/chain/generated_transaction_object.hpp;/usr/local/include/aacio/chain/genesis_state.hpp;/usr/local/include/aacio/chain/global_property_object.hpp;/usr/local/include/aacio/chain/incremental_merkle.hpp;/usr/local/include/aacio/chain/merkle.hpp;/usr/local/include/aacio/chain/multi_index_includes.hpp;/usr/local/include/aacio/chain/name.hpp;/usr/local/include/aacio/chain/permission_link_object.hpp;/usr/local/include/aacio/chain/permission_object.hpp;/usr/local/include/aacio/chain/producer_object.hpp;/usr/local/include/aacio/chain/producer_schedule.hpp;/usr/local/include/aacio/chain/protocol.hpp;/usr/local/include/aacio/chain/resource_limits.hpp;/usr/local/include/aacio/chain/resource_limits_private.hpp;/usr/local/include/aacio/chain/reversible_block_object.hpp;/usr/local/include/aacio/chain/symbol.hpp;/usr/local/include/aacio/chain/trace.hpp;/usr/local/include/aacio/chain/transaction.hpp;/usr/local/include/aacio/chain/transaction_context.hpp;/usr/local/include/aacio/chain/transaction_metadata.hpp;/usr/local/include/aacio/chain/transaction_object.hpp;/usr/local/include/aacio/chain/types.hpp;/usr/local/include/aacio/chain/wasm_aacio_binary_ops.hpp;/usr/local/include/aacio/chain/wasm_aacio_constraints.hpp;/usr/local/include/aacio/chain/wasm_aacio_injection.hpp;/usr/local/include/aacio/chain/wasm_aacio_validation.hpp;/usr/local/include/aacio/chain/wasm_interface.hpp;/usr/local/include/aacio/chain/wasm_interface_private.hpp;/usr/local/include/aacio/chain/wast_to_wasm.hpp;/usr/local/include/aacio/chain/core_symbol.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/aacio/chain" TYPE FILE FILES
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/aacio_contract.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/abi_def.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/abi_serializer.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/account_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/action.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/action_receipt.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/apply_context.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/asset.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/authority.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/authority_checker.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/authorization_manager.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_header.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_header_state.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_log.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_state.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_summary_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/block_timestamp.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/chain_config.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/chain_id_type.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/config.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/contract_table_objects.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/contract_types.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/controller.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/exceptions.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/fixed_key.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/fork_database.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/generated_transaction_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/genesis_state.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/global_property_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/incremental_merkle.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/merkle.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/multi_index_includes.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/name.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/permission_link_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/permission_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/producer_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/producer_schedule.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/protocol.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/resource_limits.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/resource_limits_private.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/reversible_block_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/symbol.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/trace.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/transaction.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/transaction_context.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/transaction_metadata.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/transaction_object.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/types.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_aacio_binary_ops.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_aacio_constraints.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_aacio_injection.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_aacio_validation.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_interface.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wasm_interface_private.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/libraries/chain/include/aacio/chain/wast_to_wasm.hpp"
    "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/libraries/chain/include/aacio/chain/core_symbol.hpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/aacio/chain/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/aacio/chain" TYPE DIRECTORY DIR_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "")
endif()

