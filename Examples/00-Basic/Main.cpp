#include <Subframe/Subframe.h>
#include <Kernel/ModuleController.h>

#include <HelloStatic.h>

int32 main(int32 argc, char ** argv)
{
	// Initialize the kernel environment

	Kernel::ModuleController controller{};

	// Load modules

	Kernel::Handle helloStatic = controller.RegisterModule(RegisterHelloStatic);

	return 0;
}
