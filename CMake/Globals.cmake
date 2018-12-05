######################
# Kernel Information #
######################

# SemVer Kernel API version
set (KERNEL_VERSION_MAJOR 0)
set (KERNEL_VERSION_MINOR 0)
set (KERNEL_VERSION_PATCH 1)

# Project build timestamp
string (TIMESTAMP KERNEL_BUILD_DATE_UTC "%Y-%m-%dT%H:%M:%SZ")

#########################
# Project Configuration #
#########################

# Root path of the project
set (KERNEL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Location for all targets' includes
set (KERNEL_INCLUDE_DIR ${KERNEL_ROOT_DIR}/Include)

# Where to place final binaries
set (KERNEL_OUTPUT_DIR ${KERNEL_ROOT_DIR}/Binaries)
