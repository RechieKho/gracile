cmake_minimum_required(VERSION 3.22)
project(gracile LANGUAGES C CXX)

# Dependencies
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    VERSION 5.0
)

FetchContent_MakeAvailable(raylib)

# Project settings
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

# Sources
set(EXECUTABLE ${PROJECT_NAME})
set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)
file(GLOB_RECURSE CODE_FILES ${CODE_DIR}/*.cpp)

add_executable(${EXECUTABLE} ${CODE_FILES})

target_link_libraries(${EXECUTABLE} 
    PRIVATE raylib
)

target_include_directories(${EXECUTABLE}
    PRIVATE ${CODE_DIR}
)