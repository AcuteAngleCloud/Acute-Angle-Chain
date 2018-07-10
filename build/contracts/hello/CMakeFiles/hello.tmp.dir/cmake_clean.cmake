file(REMOVE_RECURSE
  "hello.cpp.bc"
  "hello.bc"
  "hello.bc"
  "hello.s"
  "hello.wast"
  "hello.wasm"
  "hello.abi.hpp"
  "hello.wast.hpp"
  "CMakeFiles/hello.tmp.dir/hello.cpp.o"
  "hello.tmp.pdb"
  "hello.tmp.exe"
  "libhello.tmp.dll.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/hello.tmp.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
