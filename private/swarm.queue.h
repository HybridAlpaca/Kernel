#pragma once

#include <stdint.h>

struct loaf_task_o;

struct swarm_queue_o;

swarm_queue_o * swarm_queue_create(uint64_t capacity);

void swarm_queue_destroy(swarm_queue_o * queue);

void swarm_queue_push(swarm_queue_o * queue, loaf_task_o * task);

loaf_task_o * swarm_queue_pop(swarm_queue_o * queue);

loaf_task_o * swarm_queue_steal(swarm_queue_o * queue);
