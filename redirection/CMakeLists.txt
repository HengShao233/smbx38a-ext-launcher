project(redirection)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE ALL_RES
    ./*.h
    ./*.hpp
    ./*.c
    ./*.cpp
)
add_library(${PROJECT_NAME} SHARED ${ALL_RES})
