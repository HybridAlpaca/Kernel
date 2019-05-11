cmake_minimum_required (VERSION 3.8)

project ("kernel" VERSION "1.0.0" LANGUAGES CXX)

include (GNUInstallDirs)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

	message (FATAL_ERROR "In-tree builds are neither supported nor recommended.  See README.md for building information.")

endif ()

add_executable (${PROJECT_NAME})

if (UNIX AND NOT APPLE)

	set (PLATFORM_SOURCES "private/posix/module_loader.cpp")
	set (PLATFORM_LIBRARIES "dl" "stdc++fs" "pthread")

elseif (WINDOWS)

	set (PLATFORM_SOURCES "private/win64/module_loader.cpp")

else ()

	message ("Apple is not supported as a bulid platform.")

endif ()

target_sources (${PROJECT_NAME}
PRIVATE
	"private/main.cpp"
	"private/api-locator.cpp"
	"private/swarm.cpp"
	"private/swarm.queue.cpp"
	${PLATFORM_SOURCES}
)

target_include_directories (${PROJECT_NAME}
PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries (${PROJECT_NAME}
PRIVATE
	${PLATFORM_LIBRARIES}
)

set_target_properties (${PROJECT_NAME}
PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	PUBLIC_HEADER "public/service-locator.h;public/taskman.h"
)

install (
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-targets

	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"

	LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install (
	EXPORT ${PROJECT_NAME}-targets
	NAMESPACE loaf::

	FILE ${PROJECT_NAME}-config.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
