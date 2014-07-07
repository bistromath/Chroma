######################################################################
# Main project
######################################################################
cmake_minimum_required(VERSION 2.8.9)
project(Chroma)
enable_language(CXX)
enable_testing()
set(CHROMA_MAJOR_VERSION 0)
set(CHROMA_MINOR_VERSION 1)
set(CHROMA_PATCH_VERSION 0)
set(CHROMA_VERSION ${CHROMA_MAJOR_VERSION}.${CHROMA_MINOR_VERSION}.${CHROMA_PATCH_VERSION})

######################################################################
# Set build type default
######################################################################
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type not specified, defaulting to Release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

set(CHROMA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CHROMA_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND CHROMA_INCLUDE_DIRS
    ${CHROMA_SOURCE_DIR}/include
)

######################################################################
# Set CMake modules
######################################################################
set(CHROMA_MODULE_PATH ${CHROMA_SOURCE_DIR}/cmake/Modules)
set(CMAKE_MODULE_PATH ${CHROMA_MODULE_PATH})

include(ChromaStandardFlags)

#Check for Euler library (or whatever you end up using)
#TODO
add_definitions(-DEIGEN_MATRIX_PLUGIN="Chroma/Matrix_initializer_list.h")

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(doc)
add_subdirectory(test)
