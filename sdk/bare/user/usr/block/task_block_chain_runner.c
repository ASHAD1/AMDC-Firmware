#ifdef APP_BLOCK

#include "usr/block/task_block_chain_runner.h"
#include "usr/block/block_type.h"
#include "sys/defines.h"
#include "sys/scheduler.h"
#include <stdlib.h>

typedef struct block_chain_t {
	block_t **head;
	size_t length;
} block_chain_t;

#define MAX_NUM_BLOCK_CHAINS (8)
static block_chain_t block_chains[MAX_NUM_BLOCK_CHAINS] = {0};
static size_t num_block_chains = 0;

static task_control_block_t tcb;

void task_block_chain_runner_init(void)
{
    scheduler_tcb_init(&tcb, task_block_chain_runner_callback, NULL, "block chain runner", TASK_BLOCK_CHAIN_RUNNER_INTERVAL_USEC);
    scheduler_tcb_register(&tcb);
}

void task_block_chain_runner_callback(void *arg)
{
	for (size_t bc = 0; bc < num_block_chains; bc++) {
		block_chain_t *block_chain = &block_chains[bc];

		for (size_t b = 0; b < block_chain->length; b++) {
			block_t *block = block_chain->head[b];
			block->runner(block);
		}
	}
}

void task_block_chain_runner_register(block_t **head, size_t length)
{
	if (num_block_chains >= MAX_NUM_BLOCK_CHAINS) {
		HANG;
	}

	block_chains[num_block_chains].head = head;
	block_chains[num_block_chains].length = length;

	num_block_chains++;
}

#endif // APP_BLOCK
