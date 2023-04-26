cmake_minimum_required(VERSION 3.15)
project(frogger_project C CXX)


add_executable(tt main.cpp)
set_property(TARGET tt PROPERTY CXX_STANDARD 20)

# box_renderer.h is here
set(BOX_RENDERER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/box_renderer/source")
set(BOX_RENDERER_LIBRARY box_renderer)

target_include_directories(tt PRIVATE ${BOX_RENDERER_INCLUDE_DIR})

add_subdirectory(deps)
# box_renderer library is defined 
target_link_libraries(tt PRIVATE box_renderer)

####### from box_renderer/samples

#function(MakeThirdPartyDemo TARGETNAME FILENAME)
#    add_executable(${TARGETNAME} ${FILENAME})
#    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 20)
#    set_property(TARGET ${TARGETNAME} PROPERTY FOLDER samples)
#    target_link_libraries(${TARGETNAME} PRIVATE ${THIRD_PARTY_LIBRARIES})
#    target_include_directories(${TARGETNAME} PRIVATE ${THIRD_PARTY_INCLUDE_DIRECTORIES})
#endfunction(MakeThirdPartyDemo)

#MakeThirdPartyDemo(test_opengl test_opengl.cpp)
#MakeThirdPartyDemo(test_cpp test_cpp.cpp)

#function(MakeBoxRendererDemo TARGETNAME FILENAME)
#    add_executable(${TARGETNAME} ${FILENAME})
#    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 20)
#    set_property(TARGET ${TARGETNAME} PROPERTY FOLDER samples)
#    target_link_libraries(${TARGETNAME} PRIVATE ${BOX_RENDERER_LIBRARY})
#    target_include_directories(${TARGETNAME} PRIVATE ${BOX_RENDERER_INCLUDE_DIRECTORIES})
#endfunction(MakeBoxRendererDemo)

#MakeBoxRendererDemo(hello_box_renderer hello_box_renderer.cpp)

###### from box_renderer

#set(THIRD_PARTY_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include"
#									"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
#									"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm")
#set(THIRD_PARTY_LIBRARIES glfw glad)

#set(BOX_RENDERER_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/source")
#set(BOX_RENDERER_LIBRARY box_renderer)

#add_subdirectory(third_party)
#add_subdirectory(source)
#add_subdirectory(samples)