#include <HelloStatic.h>
#include <iostream>

void RegisterHelloStatic(ModuleDesc * desc)
{
	std::cout << "[Module] :: Inside static registration" << '\n';

	desc -> name   = "Hello Static";
	desc -> author = "HybridAlpaca";
	desc -> version.major = 0;
	desc -> version.minor = 0;
	desc -> version.patch = 0;
}
