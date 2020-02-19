#ifdef APP_BLOCK

#include "usr/block/block_const.h"
#include <usr/block/block_interface_type.h>
#include "sys/debug.h"
#include "sys/defines.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_const_io_value(block_interface_t *block_inf, void *ctx_in);
static void block_const_runner(block_interface_t *block_inf, void *ctx_in);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_const_ctx_t {
	double value;
} block_const_ctx_t;

static block_const_ctx_t block_const_ctxs[MAX_NUM_CONST_BLOCKS] = {0};
static block_interface_t block_const_interfaces[MAX_NUM_CONST_BLOCKS] = {0};

block_interface_t *block_const_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_CONST_BLOCKS) {
		return NULL;
	}

	return &block_const_interfaces[idx];
}

void *block_const_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_CONST_BLOCKS) {
		return NULL;
	}

	return &block_const_ctxs[idx];
}

void block_const_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_CONST_BLOCKS; i++) {
		block_interface_t *b = &block_const_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 1;
		b->outputs[0].exec = block_const_io_value;
		b->outputs[0].ctx = &block_const_ctxs[i];

		// Configure run function
		// ...
		b->runner = block_const_runner;
	}
}

void block_const_config_set_value(size_t idx, double value)
{
	block_const_ctxs[idx].value = value;
}

// =================

static void block_const_runner(block_interface_t *block_inf, void *ctx_in)
{
	// This runner function doesn't do anything!
}

static void *block_const_io_value(block_interface_t *block_inf, void *ctx_in)
{
	block_const_ctx_t *ctx = (block_const_ctx_t *) ctx_in;
	return &ctx->value;
}

#endif // APP_BLOCK
