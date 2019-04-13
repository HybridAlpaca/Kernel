#include <kernel.h>
#include "registry-impl.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

struct hash_t
{
   bool operator()(const char * a, const char * b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

struct registry_o
{
	std::map<const char *, const api_desc_t *, hash_t> name_desc_map;
	std::map<const char *, std::vector<const char *>, hash_t> name_dependant_map;

	std::vector<std::pair<const char *, const api_desc_t *>> waiting_list;

	std::vector<const char *> ordered_api_list;
};

void add_api(const char * name, const api_desc_t * desc);
void * get_api(const char * name);
void remove_api(const char * name);
void update_all();
void stop_all();

static registry_o state{};

static registry_api registry
{
	add_api,
	get_api,
	remove_api,
	update_all,
	stop_all
};

registry_api * get_registry()
{
	return &registry;
}

void resolve_add_dependencies()
{
	bool should_recurse{false};

	state.waiting_list.erase(std::remove_if(state.waiting_list.begin(), state.waiting_list.end(), [&](const std::pair<const char *, const api_desc_t *> & api) -> bool
	{
		for (uint16_t i{0}; i < api.second->dep_count; ++i)
		{
			if (state.name_desc_map.count(api.second->deps[i]) == 0)
			{
				return false;
			}
		}

		if (api.second->start)
		{
			api.second->start(&registry);
		}

		state.name_desc_map[api.first] = api.second;

		for (uint16_t i{0}; i < api.second->dep_count; ++i)
		{
			state.name_dependant_map[api.second->deps[i]].push_back(api.first);
		}

		should_recurse = true;

		return true;

	}), state.waiting_list.end());

	if (should_recurse)
	{
		resolve_add_dependencies();
	}
}

void resolve_update_dependencies(const char * name)
{
	for (const auto & dependant : state.name_dependant_map[name])
	{
		resolve_update_dependencies(dependant);
	}

	state.name_desc_map[name]->update(&registry);
}

void recurse_dependencies(const char * api_name)
{
	state.ordered_api_list.push_back(api_name);

	for (const auto & dependant : state.name_dependant_map[api_name])
	{
		recurse_dependencies(dependant);
	}
}

void sort_dependencies()
{
	for (const auto & api : state.name_desc_map)
	{
		if (api.second->dep_count == 0)
		{
			recurse_dependencies(api.first);
		}
	}

	std::set<const char *> unique_api_names{};

	for (const auto & api : state.ordered_api_list)
	{
		unique_api_names.insert(api);
	}

	state.ordered_api_list.assign(unique_api_names.begin(), unique_api_names.end());
}

void add_api(const char * name, const api_desc_t * desc)
{
	state.waiting_list.push_back(std::make_pair(name, desc));

	resolve_add_dependencies();
	sort_dependencies();
}

void * get_api(const char * name)
{
	if (state.name_desc_map.count(name) == 0)
	{
		return nullptr;
	}

	return state.name_desc_map[name]->api;
}

void remove_api(const char * name)
{
	//
}

void update_all()
{
	for (const auto & api_name : state.ordered_api_list)
	{
		state.name_desc_map[api_name]->update(&registry);
	}
}

void stop_all()
{
	for (auto it = state.ordered_api_list.rbegin(); it != state.ordered_api_list.rend(); ++it)
	{
		state.name_desc_map[*it]->stop(&registry);
	}
}
