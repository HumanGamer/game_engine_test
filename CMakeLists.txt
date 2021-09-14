cmake_minimum_required(VERSION 3.20)
project(engine_test)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/SDL)
add_subdirectory(lib/bgfx)

include_directories(engine_test lib/SDL/include)
include_directories(engine_test lib/bgfx/bgfx/include)
include_directories(engine_test lib/bgfx/bimg/include)
include_directories(engine_test lib/bgfx/bx/include)
add_executable(engine_test main.cpp)
target_link_libraries(engine_test SDL2-static SDL2main bgfx)
