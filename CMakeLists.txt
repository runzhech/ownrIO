cmake_minimum_required(VERSION 3.15)
project(ownrIO LANGUAGES CXX)


set(PYBIND ${CMAKE_SOURCE_DIR}/third_party/pybind11-2.5.0)


add_subdirectory(${PYBIND})
pybind11_add_module(ownrecord
    ownrecord.cpp
    ownrecord.h

)

