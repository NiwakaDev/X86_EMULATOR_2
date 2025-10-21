cmake_minimum_required(VERSION 3.26.0)

find_package(SDL2 CONFIG REQUIRED)

project(X86_EMULATOR)

file(GLOB SOURCES src/*.cpp)

add_executable(x86
    ${SOURCES}
)

target_link_libraries(x86 ${SDL2_LIBRARIES})
target_include_directories(x86 PUBLIC
    ${SDL2_INCLUDE_DIRS} 
)
target_include_directories(x86 PRIVATE include SDL2)

include(CTest)
add_subdirectory(test)
