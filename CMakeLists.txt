cmake_minimum_required(VERSION 3.13)

project(NetSim)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        )

add_executable(NetSim__debug ${SOURCE_FILES} main.cpp)


set(SOURCES_FILES_TESTS
        test/test_storage.cpp)

add_executable(NetSim__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/test_storage.cpp)
