cmake_minimum_required(VERSION 3.10)

project(colorimg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../Includer)

set(
    SOURCES
        ../Includer/Variables.cpp
        ../Includer/Tools.cpp
        ${PROJECT_SOURCE_DIR}/main/main.cpp
)

add_executable(main ${SOURCES})

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/main
)

add_custom_target(run
    DEPENDS ../main/main.cpp
    COMMAND ../main/Debug/main
    COMMENT "Auto runing main..."
    WORKING_DIRECTORY ${CMAKE_SOURCES_DIR}../main
)