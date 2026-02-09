cmake_minimum_required(VERSION 3.16)

project(AsciiDungeonGenerator
    VERSION 1.0
    LANGUAGES C CXX
)

#can set from command line by cmake -D VARNAME=value
option(BUILD_DEMO "Build demo" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type if none specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


set(library_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    CACHE PATH "Where to put built libraries"
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${library_dir})

message( STATUS ${library_dir} )

# if we're in debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -ggdb)
endif()

set(CMAKE_CXX_STANDARD 14)
include_directories("include")

add_subdirectory(src)

if(${build_demo})
    add_subdirectory(demo)
endif()