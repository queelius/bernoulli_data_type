cmake_minimum_required (VERSION 3.12)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(RandomApproximateSetModel VERSION 0.1
    DESCRIPTION "A C++ interface for the random approximate set model"
    LANGUAGES CXX)

add_subdirectory(src)
#add_subdirectory(tests)
