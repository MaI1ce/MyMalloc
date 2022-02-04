# ~/study/my_malloc/lib

cmake_minimum_required(VERSION 3.18)

set(LIB_NAME libMalloc)

set(SOURCES myMalloc.c)

set(TEST_SOURCES myMalloc.c sbrk_stubs.c)

add_library(${LIB_NAME} STATIC ${SOURCES})
#add_library(testLibMalloc STATIC ${TEST_SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(testLibMalloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
