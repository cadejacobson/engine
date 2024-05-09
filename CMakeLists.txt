cmake_minimum_required(VERSION 3.26)
project(CEngine C)

set(CMAKE_C_STANDARD 11)

# Find SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(CEngine main.c)

# Link SDL2 library
target_link_libraries(CEngine ${SDL2_LIBRARIES})




