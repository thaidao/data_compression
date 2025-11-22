cmake_minimum_required(VERSION 3.14)
project(simple_compress C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Add the main library
add_library(simple_compress_lib simple_compress.c)

# Add the executable
add_executable(simple_compress_exe simple_compress.c)
set_target_properties(simple_compress_exe PROPERTIES OUTPUT_NAME simple_compress)

# Enable testing
enable_testing()

# Download and configure GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(simple_compress_test test/simple_compress_test.cpp)
target_link_libraries(simple_compress_test 
  simple_compress_lib
  gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(simple_compress_test)
