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

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../detours/lib.X86/detours.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../detours/lib.X86/syelog.lib)
