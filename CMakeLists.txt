cmake_minimum_required(VERSION 3.25)

project(gravito
        VERSION 1.0.0
        LANGUAGES CXX       
        DESCRIPTION "a gravity based game")

set(CMAKE_CXX_STANDARD         14)
set(CMAKE_C_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_FEATURES         OFF)

set(TARGET_NAME gravito)


add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
