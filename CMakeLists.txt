cmake_minimum_required(VERSION 3.15)
project(frogger_project C CXX)


add_executable(frogger_lite "source/main.cpp")
set_property(TARGET frogger_lite PROPERTY CXX_STANDARD 20)

# we include miniaudio.h directly, as it doesn't have a .cpp file to make a library
set(MINIAUDIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/miniaudio")
# box_renderer.h is here
set(BOX_RENDERER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/box_renderer/source")

add_subdirectory(deps)

target_include_directories(frogger_lite PRIVATE ${BOX_RENDERER_INCLUDE_DIR})
target_include_directories(frogger_lite PRIVATE ${MINIAUDIO_INCLUDE_DIR})
# box_renderer library is defined in box_renderer subdirectory
target_link_libraries(frogger_lite PRIVATE box_renderer)