project(detours VERSION 4.0.1 LANGUAGES CXX)

# 用户可配置选项
option(DETOURS_DEBUG "Enable Detours debug mode" OFF)
option(DETOURS_SOURCE_BROWSING "Generate source browsing information" OFF)
option(DETOURS_ANALYZE "Enable static analysis" OFF)
option(DETOURS_WIN_7 "Target Windows 7 SDK" OFF)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 目标处理器架构检测
if(NOT DEFINED DETOURS_TARGET_PROCESSOR)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
        set(DETOURS_TARGET_PROCESSOR "x64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "X86")
        set(DETOURS_TARGET_PROCESSOR "x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64")
        set(DETOURS_TARGET_PROCESSOR "ARM64")
    else()
        set(DETOURS_TARGET_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
    endif()
endif()

# 全局编译选项
add_compile_options(
    /nologo
    /W4 /WX
    /we4777 /we4800
    /Zi /Gy /Gm- /Zl
)

# 调试宏定义
add_compile_definitions(DETOUR_DEBUG=$<BOOL:${DETOURS_DEBUG}>)

# 条件编译选项
if(DETOURS_SOURCE_BROWSING)
    add_compile_options(/FR)
else()
    add_compile_definitions(WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
endif()

# 处理器特定选项
if(DETOURS_TARGET_PROCESSOR STREQUAL "IA64")
    add_compile_options(/wd4163)  # Disable intrinsic warning
elseif(DETOURS_TARGET_PROCESSOR STREQUAL "ARM64EC")
    add_compile_options(/arm64EC)
endif()

# Windows 7 SDK 兼容性
if(DETOURS_WIN_7 AND (MSVC_VERSION GREATER_EQUAL 1700))
    add_compile_definitions(_USING_V110_SDK71_)
elseif(DETOURS_ANALYZE)
    add_compile_options(/analyze)
endif()

# 静态库目标
add_library(detours STATIC
    detours/src/detours.cpp
    detours/src/modules.cpp
    detours/src/disasm.cpp
    detours/src/image.cpp
    detours/src/creatwth.cpp
    # detours/src/uimports.cpp 被 creatwth.cpp 包含，无需单独添加
)

target_link_directories(${PROJECT_NAME} PUBLIC ./include)

# 头文件处理
configure_file(detours/src/detours.h ${CMAKE_CURRENT_SOURCE_DIR}/include/detours.h COPYONLY)
configure_file(detours/src/detver.h ${CMAKE_CURRENT_SOURCE_DIR}/include/detver.h COPYONLY)

# 库链接选项
set_target_properties(detours PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    COMPILE_PDB_NAME detours
    COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if(DETOURS_TARGET_PROCESSOR STREQUAL "ARM64EC")
    set_target_properties(detours PROPERTIES STATIC_LIBRARY_OPTIONS "/machine:arm64x")
endif()
