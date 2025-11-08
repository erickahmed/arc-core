cmake_minimum_required(VERSION 3.14)

set(project_name "arc-core-occt")
project(${project_name})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCASCADE REQUIRED)

add_library(occt SHARED
    ./point.cpp
)

target_include_directories(occt PUBLIC ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(occt PUBLIC ${OpenCASCADE_LIBRARIES})
