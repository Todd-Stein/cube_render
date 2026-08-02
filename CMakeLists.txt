cmake_minimum_required(VERSION 3.10.0)
project(cube_render VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL3 REQUIRED SDL3-shared)

if(NOT DEFINED SDL3_FOUND)
    message(FATAL_ERROR "Unable to find SDL3, please install on your system.")
endif()

find_package(GLEW REQUIRED)

message(${GLEW_INCLUDE_DIRS})

if(NOT DEFINED GLEW_FOUND)
    message(FATAL_ERROR "Unable to find GLEW, please install on your system.")
endif()

include_directories("./include/")

add_executable(cube_render main.cpp)



#target_include_directories(SDL3gl_boilerplate PRIVATE GLEW_INCLUDE_DIRS)

target_link_libraries(cube_render PRIVATE SDL3::SDL3)
target_link_libraries(cube_render PRIVATE GLEW::GLEW)


include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#SET(CMAKE_MSVC_RUNTIME_LIBRARIES "MultiThreaded")
include(CPack)
