cmake_minimum_required (VERSION 3.12)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(RandomApproximateSetModel VERSION 0.1
    DESCRIPTION "A C++ interface for the random approximate set model"
    LANGUAGES CXX)


add_library(codec INTERFACE)
#target_sources(codec INTERFACE $<BUILD_INTERFACE:./include/codec.tpp>)
#target_compile_options(codec INTERFACE -fconcepts)

#enable_testing()



add_subdirectory(src)
#add_subdirectory(tests)
