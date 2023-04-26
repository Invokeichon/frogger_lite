cmake_minimum_required(VERSION 3.15)
project(frogger_project C CXX)


add_executable(tt main.cpp )
set_property(TARGET tt PROPERTY CXX_STANDARD 20)

# box_renderer.h is here
set(BOX_RENDERER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/box_renderer/source")
set(BOX_RENDERER_LIBRARY box_renderer)

target_include_directories(tt PRIVATE ${BOX_RENDERER_INCLUDE_DIR})

add_subdirectory(deps)
# box_renderer library is defined 
target_link_libraries(tt PRIVATE box_renderer)

####### from box_renderer/samples


