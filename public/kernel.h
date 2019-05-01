/**
 * @file kernel.h
 * @author StickyFingies
 *
 * @brief Kernel API
 *
 * @version 0.0.1
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019
 */

#pragma once

#include <stdint.h>

/**
 * @struct loaf_kernel_api
 *
 * @brief Functions necessary to communicate with Loaf's kernel.  This interface will be available the whole codebase, including other modules.
 */
struct loaf_kernel_i
{
	/**
	 * @brief Insert an interface into the registry
	 *
	 * @param id Name which interface may later be retrieved by
	 * @param interface Pointer to implementation of provided interface
	 *
	 * @code{.cpp}
	 * kernel->interface_registry_add(LOAF_MY_INTERFACE_ID, &my_interface_implementation);
	 * @endcode
	 */
	void (*interface_registry_add)(const char * id, void * interface);

	/**
	 * @brief Retrieve the first registry entry with the provided id
	 *
	 * @param id Name of the interface to be retrieved
	 *
	 * @return void * Implementation of the interface to be retrieved
	 *
	 * @warning Retrieving interface which is not in registry is UB.  Query its existence using `interface_registry_count` first.
	 *
	 * @code{.cpp}
	 * auto render = static_cast<loaf_render_i *>(kernel->interface_registry_first(LOAF_RENDER_INTERFACE_ID));
	 * @endcode
	 */
	void * (*interface_registry_first)(const char * id);

	/**
	 * @brief Retrieve next registry entry under `id`
	 *
	 * @param id       Name of the interface which should be looked under
	 * @param previous Previous interface with name of `id`
	 *
	 * @return void * Implementation of the inteface to be retrieved, nullptr if nonexistent
	 *
	 * @code{.cpp}
	 * auto next_logger = static_cast<loaf_logger_i *>(kernel->interface_registry_next(LOAF_RENDER_INTERFACE_ID, &other_logger));
	 * if (next_logger == nullptr) { ...no more logger implementations... }
	 * @endcode
	 */
	void * (*interface_registry_next)(const char * id, void * previous);

	/**
	 * @brief Check how many interface implementations are registered under `id`
	 *
	 * @param id Name of the interface to query
	 *
	 * @return uint16_t Number of interfaces registered under `id`
	 *
	 * @code{.cpp}
	 * bool render_exists = kernel->interface_registry_has(LOAF_RENDER_INTERFACE_ID);
	 * @endcode
	 */
	uint16_t (*interface_registry_count)(const char * id);
};
