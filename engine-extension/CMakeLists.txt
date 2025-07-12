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

# 添加 HLSL 编译支持
find_program(FXC fxc.exe
    PATHS
    "$ENV{DXSDK_DIR}Utilities/bin/x86"
    "$ENV{VCToolsInstallDir}bin/Hostx64/x64"
    "$ENV{VCToolsInstallDir}bin/Hostx86/x86"
    REQUIRED
)

# 创建构建时目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../shaders)

# 递归查找所有 HLSL 文件
file(GLOB_RECURSE HLSL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.hlsl"
)

# 为每个着色器添加编译规则
foreach(hlsl_file IN LISTS HLSL_SOURCES)
    # 获取相对路径（用于保持目录结构）
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${hlsl_file})
    get_filename_component(out_dir ${rel_path} DIRECTORY)

    # 确定着色器类型
    get_filename_component(hlsl_name ${hlsl_file} NAME_WE)
    # 设置输出路径
    set(out_path ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../shaders/${hlsl_name}.cso)

    if(hlsl_name MATCHES "_vs$" OR hlsl_name MATCHES "^vert" OR hlsl_name MATCHES "^v_")
        set(profile vs_5_0)
    elseif(hlsl_name MATCHES "_ps$" OR hlsl_name MATCHES "^pix" OR hlsl_name MATCHES "^p_")
        set(profile ps_5_0)
    elseif(hlsl_name MATCHES "_cs$" OR hlsl_name MATCHES "^c_")
        set(profile cs_5_0)
    else()
        set(profile ps_5_0) # 默认像素着色器
    endif()

    # 添加自定义编译命令
    add_custom_command(
        OUTPUT ${out_path}
        COMMAND ${FXC}
            /nologo
            /T ${profile}
            /E main
            /Fo "./${out_path}"
            "${hlsl_file}"
        DEPENDS "${hlsl_file}"
        COMMENT "Compiling HLSL: ${rel_path}"
        VERBATIM
    )

    # 添加到编译列表
    list(APPEND HLSL_OBJECTS ${out_path})
endforeach()

# 创建着色器编译目标
add_custom_target(CompileShaders ALL
    DEPENDS ${HLSL_OBJECTS}
)

# 添加安装规则（可选）
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/
    DESTINATION shaders
    FILES_MATCHING PATTERN "*.cso"
)

# 确保在构建主目标前编译着色器
add_dependencies(${PROJECT_NAME} CompileShaders)
