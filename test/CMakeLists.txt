cmake_minimum_required(VERSION 3.26.0)

project(x86_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(memory_tests ${CMAKE_SOURCE_DIR}/src/Memory.cpp ${CMAKE_SOURCE_DIR}/src/Object.cpp MemoryTests.cpp)
target_link_libraries(memory_tests GTest::gtest_main)
target_include_directories(memory_tests PRIVATE ../include)
add_test(NAME memory_tests COMMAND memory_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
