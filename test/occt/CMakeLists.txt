cmake_minimum_required(VERSION 3.14)
project(arc-core-test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)

add_executable(tests
    /app/test/point-test.cpp
    /app/src/point.cpp
)

target_include_directories(tests PRIVATE
    /app/src/include
    /usr/local/include/opencascade
)

target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    /usr/local/lib/libocct.so
)

include(CTest)
include(Catch)

catch_discover_tests(tests)
