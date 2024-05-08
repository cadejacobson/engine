cmake_minimum_required(VERSION 3.26)
project(engine)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add your source files
add_executable(engine main.cpp)

# Link SDL2 library
target_link_libraries(engine ${SDL2_LIBRARIES})
