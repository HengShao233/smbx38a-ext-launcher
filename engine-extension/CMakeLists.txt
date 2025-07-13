add_subdirectory(render/shaders)

project(engine-extension)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE ALL_RES
    ./*.h
    ./*.hpp
    ./*.c
    ./*.cpp
)
include_directories(./ ../detours/include)

add_library(${PROJECT_NAME} SHARED ${ALL_RES})
target_link_libraries(${PROJECT_NAME} PRIVATE detours)

# 确保在构建主目标前编译着色器
add_dependencies(${PROJECT_NAME} shaders)
