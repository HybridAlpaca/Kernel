# Kernel

add_library (Kernel STATIC)

target_sources (Kernel
PRIVATE
	Kernel/SharedLib.cpp
	Kernel/WorkQueue.cpp
	Kernel/Task.cpp
	Kernel/Remotery.cpp
	Kernel/Scheduler.cpp
	Kernel/POSIX/Window.cpp
	Kernel/POSIX/Context.cpp
	Kernel/OpenGL/Model.cpp
)

target_compile_options     (Kernel PRIVATE -g) # debug mode only
set_target_properties      (Kernel PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_include_directories (Kernel PUBLIC ${KERNEL_INCLUDE_DIR})
target_link_libraries      (Kernel PRIVATE dl pthread X11 GL GLEW assimp)
