cmake_minimum_required(VERSION 3.27)
project(AGENTS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/.venv/share/cmake/pybind11")

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 11)
include_directories("${CMAKE_SOURCE_DIR}/headers")

pybind11_add_module(agents 
    src/agents.cpp 
    src/MCTSAgent.cpp 
    src/Environment.cpp 
    src/NAgent.cpp
)

set_target_properties(agents PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )