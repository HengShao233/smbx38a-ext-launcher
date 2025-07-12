cmake_minimum_required(VERSION 3.24.0)

project(smbx38a-ext-launcher)

if(NOT WIN32 OR NOT MSVC)
    message(FATAL_ERROR  "${PROJECT_NAME}::CMakeLists::Error: Unsupported platform")
endif()

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(main)
add_subdirectory(redirection)
add_subdirectory(engine-extension)
