cmake_minimum_required(VERSION 3.0.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debug symbols by default
# Must be done before project() statement
# Can also set it on the command line: -DCMAKE_BUILD_TYPE=Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(lazarus)

include(CTest)

set(LIBRARY_NAME "lazarus")

file(GLOB_RECURSE SOURCES src/lazarus/*)

add_library(${LIBRARY_NAME} SHARED ${SOURCES})

# Windows specifics
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    # For now, SFML has to be located in the game's root directory
    set(SFML_DIR ${PROJECT_SOURCE_DIR}/../SFML/lib/cmake/SFML)
    set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SFML/include)
endif()

# SFML
find_package(SFML 2.5 COMPONENTS graphics window audio REQUIRED)
if(SFML_FOUND)
    # Only the highest level is needed, other dependencies (windows, system, etc)
    # are automatically resolved
    target_include_directories(${LIBRARY_NAME} PRIVATE ${SFML_INCLUDE_DIR})
    target_link_libraries(${LIBRARY_NAME} sfml-graphics stdc++fs)
else()
    message(FATAL_ERROR "SFML was not found.")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)

include_directories(src)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lazarus/src)
