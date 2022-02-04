# ~/study/my_malloc/ 

cmake_minimum_required(VERSION 3.18)

project(MyMalloc)

set(SOURCE main.c)

set(TEST OFF CACHE BOOL "library setting")

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} libMalloc)
