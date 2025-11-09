cmake_minimum_required(VERSION 3.14)
project(arc-core-occt-test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCASCADE REQUIRED)
find_package(Catch2 REQUIRED)

add_library(occt_point SHARED
    ../src/point.cpp
)

target_include_directories(occt_point PUBLIC
    ../src/include
    ${OpenCASCADE_INCLUDE_DIRS}
)

target_link_libraries(occt_point PUBLIC
    TKernel
    TKMath
    TKBRep
    TKGeomBase
    TKPrim
)

add_executable(tests
    point-test.cpp
)

target_include_directories(tests PRIVATE
    ../src/include
    ../src
    ${OpenCASCADE_INCLUDE_DIRS}
)

target_link_libraries(tests PRIVATE
    occt_point
    TKernel
    TKMath
    TKBRep
    TKGeomBase
    TKPrim
    Catch2::Catch2WithMain
)
