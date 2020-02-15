#ifdef APP_BLOCK

#include "usr/block/block_print_int.h"
#include "usr/block/block_type.h"
#include "sys/debug.h"
#include "sys/defines.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void block_print_int_runner(block_t *block);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_print_int_state_t {
	char *prepend_msg;
} block_print_int_state_t;

static block_print_int_state_t block_print_int_state[MAX_NUM_PRINT_INT_BLOCKS] = {0};
static block_t block_print_int_blocks[MAX_NUM_PRINT_INT_BLOCKS] = {0};

block_t *block_print_int_get(size_t idx)
{
	if (idx >= MAX_NUM_PRINT_INT_BLOCKS) {
		return NULL;
	}

	return &block_print_int_blocks[idx];
}

void block_print_int_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_PRINT_INT_BLOCKS; i++) {
		block_t *b = block_print_int_get(i);

		// Configure inputs
		// ...
		b->num_inputs = 1;

		// Configure outputs
		// ...
		b->num_outputs = 0;

		// Configure state pointer
		// ...
		b->state = &block_print_int_state[i];

		// Configure run function
		// ...
		b->runner = block_print_int_runner;
	}
}

void block_print_int_config_prepend(size_t idx, char *msg)
{
	block_print_int_state[idx].prepend_msg = msg;
}

// =================

static void block_print_int_runner(block_t *block)
{
	block_print_int_state_t *state = (block_print_int_state_t *) block->state;

	// Retrieve input as integer
	int out = *((int *) block->inputs[0](NULL));

	debug_printf("%s%d\r\n", state->prepend_msg, out);
}

#endif // APP_BLOCK
