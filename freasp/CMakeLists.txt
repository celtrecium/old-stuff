cmake_minimum_required(VERSION 3.10)

project(freasp)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# set(CMAKE_C_COMPILER "/bin/musl-clang")

add_compile_options(-std=c89 -pedantic -Wall -Wextra -Wsign-conversion
  -Wconversion -Wshadow)


include_directories(include/)

file(GLOB
  SRCS "src/*.c")

add_executable(${PROJECT_NAME} ${SRCS})
