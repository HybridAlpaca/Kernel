cmake_minimum_required (VERSION 3.13.0)
project (Kernel CXX)

set     (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
include (Globals)

add_subdirectory (Source)
add_subdirectory (Examples)
