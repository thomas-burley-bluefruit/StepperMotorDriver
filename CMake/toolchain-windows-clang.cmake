# Clang on Windows toolchain initialization
set(CMAKE_C_COMPILER "clang-cl")
set(CMAKE_CXX_COMPILER "clang-cl")
set(CMAKE_LINKER "lld-link")

set(CMAKE_CXX_FLAGS "-m32 /EHsc")
set(CMAKE_C_FLAGS "-m32")
