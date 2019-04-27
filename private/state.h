#pragma once

#include <stdint.h>

struct module_desc_t;

// API registry

void api_impl_add(const char * name, void * impl);

void * api_impl_first(const char * name);

void * api_impl_next(const char * name, void * prev);

// Modules

void load_modules_dir(const char * path);

void unload_modules();

uint16_t get_loaded_modules(module_desc_t *** modules);

// Modules (@todo merge)

void registry_add_module(const module_desc_t * module);

void registry_start_modules();

void registry_stop_modules();

void registry_execute_modules();

void * registry_get_module(const char * id);
