file(REMOVE_RECURSE
  "hello.cpp.bc"
  "hello.bc"
  "hello.bc"
  "hello.s"
  "hello.wast"
  "hello.wasm"
  "hello.abi.hpp"
  "hello.wast.hpp"
  "CMakeFiles/hello"
  "hello.wast.hpp"
  "hello.abi.hpp"
  "hello.wasm"
  "hello.wast"
  "hello.s"
  "hello.bc"
  "hello.cpp.bc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/hello.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
