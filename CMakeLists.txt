cmake_minimum_required(VERSION 3.26)

project(bulbit LANGUAGES CXX VERSION 0.0.1)

option(BULBIT_BUILD_SAMPLES "Build Samples" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

if(MSVC)
    # default startup project for Visual Studio
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_compile_options(/MP /wd4819 /wd4996)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(extern)
add_subdirectory(src)

if(BULBIT_BUILD_SAMPLES)
    add_subdirectory(sample)
endif()