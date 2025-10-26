cmake_minimum_required(VERSION 3.29)
project(HyperEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(HYPER_RESOURCE_PATH ${CMAKE_SOURCE_DIR}/Resources)

option(BUILD_TEST "Option to build tests" ON)

execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE HYPER_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
)

execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE HYPER_GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
)

add_subdirectory(Engine)

if(BUILD_TEST)
    add_subdirectory(Test)
endif ()