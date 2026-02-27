cmake_minimum_required(VERSION 3.10)

#设置项目名称
project(TestProject)

#设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#告诉cmake包含头文件目录
#${PROJECT_SOURCE_DIR}CMakeLists.txt在项目文件夹下
include_directories(${PROJECT_SOURCE_DIR}/header)

#收集所有源文件(.cpp)
#包括Src下的Variables.cpp 和 根目录下的NonDefine.cpp
set(
    SOURCES
        ${PROJECT_SOURCE_DIR}/src/Variables.cpp
        ${PROJECT_SOURCE_DIR}/NoneDefine/NonDefine.cpp
)


#生成可执行文件(.exe)
add_executable(nodefine ${SOURCES})

#配置生成位置
#${CMAKE_BINARY_DIR}是build文件夹,但build并不是固定名称,和一开始build cmake文件有关
set_target_properties(nodefine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/NoneDefine
)


#生成完毕可执行文件后执行
add_custom_target(run
    DEPENDS ../NoneDefine/NonDefine
    COMMAND ../NoneDefine/nodefine
    COMMENT "Auto runing nodefing..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

 
