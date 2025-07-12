project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE ALL_RES
    ./*.h
    ./*.hpp
    ./*.c
    ./*.cpp
)
file(GLOB_RECURSE ALL_RC "./*.rc")

include_directories(./ ../detours/include)
add_executable(${PROJECT_NAME} ${ALL_RES})
target_sources(${PROJECT_NAME} PRIVATE ${ALL_RC})

target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
