cmake_minimum_required (VERSION 3.8)

set (MODULE_NAME "kernel")
set (MODULE_VERSION "0.0.1")

project (${MODULE_NAME} CXX)

include (GNUInstallDirs)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

	message (FATAL_ERROR "In-tree builds are neither supported nor recommended.  See README.md for building information.")

endif ()

add_executable (${MODULE_NAME})

target_sources (${MODULE_NAME}
PRIVATE
	"source/main.cpp"
	"source/module-registry.cpp"
	"source/api-registry.cpp"

	"source/posix/module-loader.cpp"
)

target_include_directories (${MODULE_NAME}
PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries (${MODULE_NAME}
PRIVATE
	dl
	stdc++fs
)

set_target_properties (${MODULE_NAME}
PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	PUBLIC_HEADER "include/${MODULE_NAME}.h"
)

install (
	TARGETS ${MODULE_NAME}
	EXPORT ${MODULE_NAME}-targets

	INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install (
	EXPORT ${MODULE_NAME}-targets
	NAMESPACE gengine::

	FILE ${MODULE_NAME}-config.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${MODULE_NAME}"
)
