/**
 * @file module_loader.h
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

/**
 * @brief Loads provided file paths into memory as modules
 *
 * @param file_names Array of absolute paths to modules which should be loaded
 *
 * @return const char * Error message produced during loading process, if any
 */
const char * module_load_files(const char ** file_paths, uint16_t file_count);

/**
 * @brief Unloads all of the currently loaded modules
 */
void module_unload_all();