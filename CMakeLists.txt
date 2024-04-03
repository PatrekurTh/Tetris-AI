cmake_minimum_required(VERSION 3.27)
project(AGENTS)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/.venv/share/cmake/pybind11")

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 11)

pybind11_add_module(agents agents.cpp MCTSAgent.cpp Environment.cpp NAgent.cpp)

set_target_properties(agents PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )