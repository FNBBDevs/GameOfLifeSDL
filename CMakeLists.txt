cmake_minimum_required(VERSION 3.22)
project(GameOfLife VERSION 1.0 DESCRIPTION "Conway's Game of Life visualized using C++ and SDL2")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

message("cmake module path ${CMAKE_MODULE_PATH}")

find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        include/colors.h
        include/config.h
        include/interpolations.h
        include/kernels.h
)

target_link_libraries(GameOfLife PRIVATE SDL2::SDL2 SDL2::SDL2main)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)