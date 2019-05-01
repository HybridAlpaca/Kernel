cmake_minimum_required (VERSION 3.8)

set (MODULE_NAME "kernel")
set (MODULE_VERSION "0.0.1")

project (${MODULE_NAME} CXX)

include (GNUInstallDirs)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

	message (FATAL_ERROR "In-tree builds are neither supported nor recommended.  See README.md for building information.")

endif ()

add_executable (${MODULE_NAME})

if (UNIX AND NOT APPLE)
	set (PLATFORM_SOURCES "private/posix/module_loader.cpp")
	set (PLATFORM_LIBRARIES "dl" "stdc++fs")
elseif (WINDOWS)
	set (PLATFORM_SOURCES "private/win64/module_loader.cpp")
else ()
	message ("Sorry buster, Mac isn't supported.")
endif ()

target_sources (${MODULE_NAME}
PRIVATE
	"private/main.cpp"
	${PLATFORM_SOURCES}
)

target_include_directories (${MODULE_NAME}
PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries (${MODULE_NAME}
PRIVATE
	${PLATFORM_LIBRARIES}
)

set_target_properties (${MODULE_NAME}
PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	# PUBLIC_HEADER "public/${MODULE_NAME}.h"
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
