cmake_minimum_required(VERSION 3.25)

project(gravito
        VERSION 1.0.0
        LANGUAGES CXX       
        DESCRIPTION "a gravity based game")

set(CMAKE_CXX_STANDARD         14)
set(CMAKE_C_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_FEATURES         OFF)

set(TARGET_NAME gravito)

include(FetchContent)

set(SFML_LIB_NAME SFML)

FetchContent_Declare(
    ${SFML_LIB_NAME}
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 2.6.1
    GIT_SHALLOW FALSE 
)

FetchContent_MakeAvailable(${SFML_LIB_NAME})

add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${TARGET_NAME} PUBLIC sfml-window)
target_link_libraries(${TARGET_NAME} PUBLIC sfml-system) 
target_link_libraries(${TARGET_NAME} PUBLIC sfml-graphics)

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-window>
    $<TARGET_FILE_DIR:${TARGET_NAME}>
)

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-graphics>
    $<TARGET_FILE_DIR:${TARGET_NAME}>
)

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-system>
    $<TARGET_FILE_DIR:${TARGET_NAME}>
)