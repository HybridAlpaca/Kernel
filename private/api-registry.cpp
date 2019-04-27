#include <kernel.h>

#include <cstring>
#include <map>
#include <vector>

struct char_hash_t
{
	bool operator()(const char * a, const char * b) const
   	{
		return std::strcmp(a, b) < 0;
	}
};

struct api_registry_o
{
	std::map<const char *, std::vector<void *>, char_hash_t> name_impl_map;
};

static api_registry_o state{};

void api_impl_add(const char * name, void * impl)
{
	state.name_impl_map[name].push_back(impl);
}

void * api_impl_first(const char * name)
{
	if (state.name_impl_map.count(name) == 0)
	{
		return nullptr;
	}

	return state.name_impl_map[name][0];
}

void * api_impl_next(const char * name, void * prev)
{
	if (state.name_impl_map.count(name) == 0)
	{
		return nullptr;
	}

	for (auto it = state.name_impl_map[name].begin(); it != state.name_impl_map[name].end(); ++it)
	{
		if (*it != prev)
		{
			continue;
		}

		const auto next = ++std::move(it);

		if (next == state.name_impl_map[name].end())
		{
			return nullptr;
		}

		return *next;
	}

	return nullptr;
}
