
cmake_minimum_required(VERSION 2.10)

PROJECT(test)

include_directories(
    ${PROJECT_SOURCE_DIR}/moduleA
    ${PROJECT_SOURCE_DIR}/moduleB
)

set(SRC
    ${PROJECT_SOURCE_DIR}/test.c
)

set(SRC_moduleA
    ${PROJECT_SOURCE_DIR}/moduleA/moduleA.c
)

set(SRC_moduleB
    ${PROJECT_SOURCE_DIR}/moduleB/moduleB.c
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(LIB_A test_liba)
set(LIB_B test_libb)

add_library(${LIB_A} STATIC ${SRC_moduleA})
add_library(${LIB_B} STATIC ${SRC_moduleB})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIB_A} ${LIB_B})

# make VERBOSE=1
# set(CMAKE_VERBOSE_MAKEFILE ON)
