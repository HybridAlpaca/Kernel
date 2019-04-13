#include <kernel.h>
#include "registry-impl.h"

#include <iostream>

static api_desc_t foo
{
	0,
	nullptr,

	nullptr,
	[](registry_api * registry){ std::cout << "Starting foo" << std::endl; },
	[](registry_api * registry){ std::cout << "Stopping foo" << std::endl; },
	[](registry_api * registry){ std::cout << "Updating foo" << std::endl; }
};

static const char * bar_deps[]{ "foo", "baz" };
static api_desc_t bar
{
	2,
	bar_deps,

	nullptr,
	[](registry_api * registry){ std::cout << "Starting bar" << std::endl; },
	[](registry_api * registry){ std::cout << "Stopping bar" << std::endl; },
	[](registry_api * registry){ std::cout << "Updating bar" << std::endl; }
};

static const char * baz_deps[]{ "foo" };
static api_desc_t baz
{
	1,
	baz_deps,

	nullptr,
	[](registry_api * registry){ std::cout << "Starting baz" << std::endl; },
	[](registry_api * registry){ std::cout << "Stopping baz" << std::endl; },
	[](registry_api * registry){ std::cout << "Updating baz" << std::endl; }
};

int main()
{
	registry_api * registry = get_registry();

	registry->add_api("bar", &bar);
	registry->add_api("baz", &baz);
	registry->add_api("foo", &foo);

	registry->update_all();

	registry->stop_all();

	return 0;
}
