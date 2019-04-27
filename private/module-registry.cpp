#include "state.h"
#include <kernel.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>

extern kernel_i kernel;

struct char_hash_t
{
	bool operator()(const char * a, const char * b) const
   	{
		return std::strcmp(a, b) < 0;
	}
};

struct module_registry_o
{
	std::set<const module_desc_t *> module_cache;

	std::vector<const module_desc_t *> module_queue;

	std::set<const char *, char_hash_t> apis_exported;
};

static module_registry_o state;

void registry_add_module(const module_desc_t * mod)
{
	state.module_cache.insert(mod);
}

bool recurse_module_cache(const module_desc_t * mod)
{
	// If any one of our imported API's has not yet been exported,
	// abort immediately
	for (uint16_t i{0}; i < mod->import_count; ++i)
	{
		if (state.apis_exported.count(mod->imports[i]) == 0)
		{
			return false;
		}
	};

	// Export our API's
	for (uint16_t i{0}; i < mod->export_count; ++i)
	{
		state.apis_exported.insert(mod->exports[i]);
	}

	// Schedule ourselves for startup
	state.module_queue.push_back(mod);

	return true;
}

void enqueue_modules()
{
	bool should_recurse{false};

	for (auto it = state.module_cache.begin(); it != state.module_cache.end(); )
	{
		const bool queue_successful = recurse_module_cache(*it);

		if (queue_successful)
		{
			it = state.module_cache.erase(it);
			should_recurse = true;
		}
		else
		{
			++it;
		}

	}

	if (should_recurse)
	{
		enqueue_modules();
	}
}

void registry_start_modules()
{
	enqueue_modules();

	for (const auto & mod : state.module_queue)
	{
		mod->start(&kernel);
	}
}

void registry_stop_modules()
{
	for (auto it = state.module_queue.rbegin(); it != state.module_queue.rend(); ++it)
	{
		(*it)->stop();
	}
}

void registry_execute_modules()
{
	for (const auto & mod : state.module_queue)
	{
		mod->update();
	}
}

void * registry_get_module(const char * id)
{
	return nullptr;
}
