# Kernel

add_library (Kernel STATIC)

target_sources (Kernel
PRIVATE
	Kernel/SharedLib.cpp
	Kernel/WorkQueue.cpp
	Kernel/Task.cpp
	Kernel/Debug.cpp
	Kernel/Scheduler.cpp
	Kernel/POSIX/Window.cpp
	Kernel/POSIX/Context.cpp
	Kernel/OpenGL/Model.cpp
	Kernel/OpenGL/Shader.cpp

	Kernel/Graphics/GLShader.cpp
	Kernel/Graphics/GLProgram.cpp
	Kernel/Graphics/GLUniform.cpp

	Remotery/Remotery.cpp
)

target_compile_options     (Kernel PRIVATE -g -Wall -Werror -fno-exceptions)
set_target_properties      (Kernel PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_include_directories (Kernel PUBLIC ${KERNEL_INCLUDE_DIR})
target_link_libraries      (Kernel PRIVATE dl pthread X11 GL GLEW assimp)
