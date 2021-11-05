cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")

project(calendar_routine_test)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  irrigation_manager_test
  irrigation_manager_test.cpp
  irrigation_manager.cpp
)
target_link_libraries(
  irrigation_manager_test
  gtest_main
  gmock_main
)

include(GoogleTest)
gtest_discover_tests(irrigation_manager_test)