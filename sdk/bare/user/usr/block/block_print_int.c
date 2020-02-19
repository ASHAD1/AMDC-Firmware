
#ifdef APP_BLOCK

#include "usr/block/block_print_int.h"
#include <usr/block/block_interface_type.h>
#include "sys/debug.h"
#include "sys/defines.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void block_print_int_runner(block_interface_t *block_inf, void *ctx_in);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_print_int_ctx_t {
	char *prepend_msg;
} block_print_int_ctx_t;

static block_print_int_ctx_t block_print_int_ctxs[MAX_NUM_PRINT_INT_BLOCKS] = {0};
static block_interface_t block_print_int_interfaces[MAX_NUM_PRINT_INT_BLOCKS] = {0};

block_interface_t *block_print_int_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_PRINT_INT_BLOCKS) {
		return NULL;
	}

	return &block_print_int_interfaces[idx];
}

void *block_print_int_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_PRINT_INT_BLOCKS) {
		return NULL;
	}

	return &block_print_int_ctxs[idx];
}

void block_print_int_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_PRINT_INT_BLOCKS; i++) {
		block_interface_t *b = &block_print_int_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 1;

		// Configure outputs
		// ...
		b->num_outputs = 0;

		// Configure run function
		// ...
		b->runner = block_print_int_runner;
	}
}

void block_print_int_config_prepend(size_t idx, char *msg)
{
	block_print_int_ctxs[idx].prepend_msg = msg;
}

// =================

static void block_print_int_runner(block_interface_t *block_inf, void *ctx_in)
{
	block_print_int_ctx_t *ctx = (block_print_int_ctx_t *) ctx_in;

	// Retrieve input as integer
	int out = *((int *) block_inf->inputs[0].exec(block_inf, ctx));

	debug_printf("%s%d\r\n", ctx->prepend_msg, out);
}

#endif // APP_BLOCK
