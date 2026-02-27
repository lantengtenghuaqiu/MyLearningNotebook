cmake_minimum_required(VERSION 3.10)

project(gradual_background)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

include_directories(../Includer)

set(
    SOURCES
        ../Includer/Tools.cpp
        ../Includer/Variables.cpp
        ${PROJECT_SOURCE_DIR}/Main/main.cpp
)

add_executable(main ${SOURCES})

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Main
)

add_custom_target(run
    DEPENDS Main/main.cpp
    COMMAND ../Main/Debug/main
    COMMENT "Auto runing main..."
)
