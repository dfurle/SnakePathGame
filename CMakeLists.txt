cmake_minimum_required(VERSION 3.10.2)

set(NAME "snake.exe")

add_executable(${NAME} snake.cpp)

find_library(GRAPHICS_LIB sfml-graphics)
find_library(WINDOW_LIB sfml-window)
find_library(SYSTEM_LIB sfml-system)
find_library(AUDIO_LIB sfml-audio)
target_link_libraries(${NAME} PRIVATE "${GRAPHICS_LIB}")
target_link_libraries(${NAME} PRIVATE "${WINDOW_LIB}")
target_link_libraries(${NAME} PRIVATE "${SYSTEM_LIB}")
target_link_libraries(${NAME} PRIVATE "${AUDIO_LIB}")


target_include_directories(${NAME} PRIVATE include)
