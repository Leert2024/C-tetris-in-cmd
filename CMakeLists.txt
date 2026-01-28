CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# 本工程的名称
SET(CMAKE_PROJECT_NAME tetris)

PROJECT(${CMAKE_PROJECT_NAME} C CXX)

# 使用初始的C源文件创建可执行文件
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} src/main.c)

# 设置可执行文件的输出目录
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# 添加自定义的头文件包含目录
TARGET_INCLUDE_DIRECTORIES(${CMAKE_PROJECT_NAME} PRIVATE
    inc
)

# 添加C源文件
TARGET_SOURCES(${CMAKE_PROJECT_NAME} PRIVATE 
    src/display.c
    src/fileio.c
    src/boardop.c
)

# 设置源码和编译使用UTF-8编码
if (MSVC)
    # Windows + MSVC编译器
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_definitions(-D_UNICODE -DUNICODE)
else()
    # GCC/Clang等编译器
    add_compile_options(-finput-charset=UTF-8)
    add_compile_options(-fexec-charset=UTF-8)
    add_compile_options(-fwide-exec-charset=UTF-8)
endif()

# 设置输出为宽字符控制台
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /utf-8")
