/**
 * @file interface_registry.h
 * @author StickyFingies
 *
 * @brief Interface... for the interface registry.  See kernel.h for function documentation.
 *
 * @see kernel.h
 *
 * @version 0.0.1
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019
 */

#pragma once

#include <stdint.h>

/**
 * @see kernel.h
 */
void interface_registry_add(const char * id, void * interface);

/**
 * @see kernel.h
 */
void * interface_registry_first(const char * id);

/**
 * @see kernel.h
 */
void * interface_registry_next(const char * id, void * previous);

/**
 * @see kernel.h
 */
uint16_t interface_registry_count(const char * id);
