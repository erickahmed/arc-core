cmake_minimum_required(VERSION 3.14)
project(arc-core-occt-tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)

add_executable(tests
    point-test.cpp
)

target_include_directories(tests PRIVATE
    ../src/include
    /usr/local/include/opencascade
)

# Link the library from the build folder
target_link_libraries(tests PRIVATE
    ../build/libocct.so
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
