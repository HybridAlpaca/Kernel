/**
 * @file module_loader
 * @author StickyFingies
 *
 * @brief Module loader API
 *
 * @version 0.0.1
 * @date 2019-04-30
 *
 * @copyright Copyright (c) 2019
 */

#pragma once

#include <stdint.h>

struct module_loader_o;

struct module_loader_i
{
	/**
	 * @brief Private implementation data
	 */
	module_loader_o * inst;

	/**
	 * @brief Loads provided file paths into memory as modules
	 *
	 * @param inst       Private implementation
	 * @param file_paths Array to file paths which should be loaded
	 * @param file_count Number of paths contained within file_paths
	 *
	 * @return const char * Error message produced during loading process, if any
	 */
	const char * (*load_files)(module_loader_i * api, const char * const * file_paths, uint16_t file_count);

	/**
	 * @brief Unloads all of the currently loaded modules
	 *
	 * @param inst Private implementation
	 */
	void (*unload_all)(module_loader_i * api);
};

module_loader_i * create_default_module_loader();

void destroy_default_module_loader(module_loader_i * module_loader);
