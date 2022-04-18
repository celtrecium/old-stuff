cmake_minimum_required(VERSION 3.10)

project(forth)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-std=c99 -pedantic -Wall -Wextra -Wsign-conversion -Wconversion -Wshadow)

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)
endif()

include_directories(include/)

file(GLOB
    SRCS "src/*.c")

add_library(${PROJECT_NAME} SHARED ${SRCS})

