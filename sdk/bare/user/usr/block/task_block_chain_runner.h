#ifndef TASK_BLOCK_CHAIN_RUNNER_H
#define TASK_BLOCK_CHAIN_RUNNER_H

#include "sys/defines.h"
#include <stdlib.h>
#include <stdint.h>
#include <usr/block/block_interface_type.h>

#define TASK_BLOCK_CHAIN_RUNNER_UPDATES_PER_SEC (10000)
#define TASK_BLOCK_CHAIN_RUNNER_INTERVAL_USEC (USEC_IN_SEC / TASK_BLOCK_CHAIN_RUNNER_UPDATES_PER_SEC)

void task_block_chain_runner_init(void);
void task_block_chain_runner_callback(void *arg);

void task_block_chain_runner_register(block_t **head, size_t length);

#endif // TASK_BLOCK_CHAIN_RUNNER_H
