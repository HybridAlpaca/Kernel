/**
 * @file interface_registry.cpp
 * @author StickyFingies
 *
 * @brief Implementation of the kernel's interface registry
 *
 * @version 0.0.1
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019
 */

#include "interface_registry.h"
#include <kernel.h>

#include <cstring>
#include <map>
#include <vector>

/**
 * @struct str_hash_t
 *
 * @brief Allows using `char *` as a key in STL map structures
 */
struct str_hash_t
{
   bool operator()(const char * a, const char * b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

/**
 * @struct interface_registry_o
 *
 * @brief All internal guts and state used by the interface registry
 */
struct interface_registry_o
{
	/// @brief Associative map which allows a list of interfaces to be looked up given an ID
	std::map<const char *, std::vector<void *>, str_hash_t> name_interface_map;
};

static interface_registry_o internal{};

void interface_registry_add(const char * id, void * interface)
{
	internal.name_interface_map[id].push_back(interface);
}

void * interface_registry_first(const char * id)
{
	return internal.name_interface_map[id][0];
}

void * interface_registry_next(const char * id, void * previous)
{
	// @todo
	return nullptr;
}

uint16_t interface_registry_count(const char * id)
{
	// Check if map contains id

	if (internal.name_interface_map.count(id) == 0)
	{
		return 0;
	}

	// Return size of list

	return internal.name_interface_map[id].size();
}
