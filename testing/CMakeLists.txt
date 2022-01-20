cmake_minimum_required(VERSION 3.14)
project(testing)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Point to header files
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/..
)

# Use GoogleTest
enable_testing()
add_executable(
  google_test
  google_test.cpp
)
target_link_libraries(
  google_test
  gtest_main
  # test_class
)
include(GoogleTest)
gtest_discover_tests(google_test)