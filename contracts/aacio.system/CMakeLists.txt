file(GLOB ABI_FILES "*.abi")
configure_file("${ABI_FILES}" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)

add_wast_executable(TARGET aacio.system
  INCLUDE_FOLDERS ${STANDARD_INCLUDE_FOLDERS}
  LIBRARIES libc++ libc aaciolib aacio.token
  DESTINATION_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
)
