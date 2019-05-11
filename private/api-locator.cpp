#include <service-locator.h>

#include <map>
#include <vector>

struct api_locator_o
{
	std::map<const char *, void *> id_head_map;
	std::map<void *, void *> node_next_map;
};

static void api_locator_construct(loaf_service_locator_i * api)
{
	api->state = new api_locator_o{};
}

static void api_locator_destruct(loaf_service_locator_i * api)
{
	auto state = static_cast<api_locator_o *>(api->state);

	delete state;
}

static void api_locator_provide_service(loaf_service_locator_i * api, const char * service_id, void * service)
{
	auto state = static_cast<api_locator_o *>(api->state);

	if (state->id_head_map.count(service_id) == 0)
	{
		state->id_head_map[service_id] = service;
		state->node_next_map[service] = nullptr;
	}
	else
	{
		for (auto edge : state->node_next_map)
		{
			if (edge.second == nullptr)
			{
				edge.second = service;
				state->node_next_map[service] = nullptr;
				break;
			}
		}
	}
}

static void * api_locator_first_service(loaf_service_locator_i * api, const char * service_id)
{
	auto state = static_cast<api_locator_o *>(api->state);

	return state->id_head_map[service_id];
}

static void * api_locator_next_service(loaf_service_locator_i * api, void * previous)
{
	auto state = static_cast<api_locator_o *>(api->state);

	return state->node_next_map[previous];
}

void loaf_load_service_locator(loaf_service_locator_i * locator)
{
	// Normally, this method would use the provided `locator` argument
	// to register the interface there.  In this case, the api-locator
	// is *providing* that actual locator, so we just copy it through
	// the pointer instead.

	(*locator) =
	{
		nullptr,
		api_locator_construct,
		api_locator_destruct,
		api_locator_provide_service,
		api_locator_first_service,
		api_locator_next_service
	};
}

void loaf_unload_service_locator(loaf_service_locator_i * locator)
{
	//
}
