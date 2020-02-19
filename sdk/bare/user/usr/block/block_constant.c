#ifdef APP_BLOCK

#include "usr/block/block_constant.h"
#include <usr/block/block_interface_type.h>
#include "sys/debug.h"
#include "sys/defines.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_constant_io_value(block_interface_t *block_inf, void *ctx_in);
static void block_constant_runner(block_interface_t *block_inf, void *ctx_in);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_constant_ctx_t {
	double value;
} block_constant_ctx_t;

static block_constant_ctx_t block_constant_ctxs[MAX_NUM_CONSTANT_BLOCKS] = {0};
static block_interface_t block_constant_interfaces[MAX_NUM_CONSTANT_BLOCKS] = {0};

block_interface_t *block_constant_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_CONSTANT_BLOCKS) {
		return NULL;
	}

	return &block_constant_interfaces[idx];
}

void *block_constant_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_CONSTANT_BLOCKS) {
		return NULL;
	}

	return &block_constant_ctxs[idx];
}

void block_constant_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_CONSTANT_BLOCKS; i++) {
		block_interface_t *b = &block_constant_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 1;
		b->outputs[0].exec = block_constant_io_value;
		b->outputs[0].ctx = &block_constant_ctxs[i];

		// Configure run function
		// ...
		b->runner = block_constant_runner;
	}
}

void block_constant_config_set_value(size_t idx, double value)
{
	block_constant_ctxs[idx].value = value;
}

// =================

static void block_constant_runner(block_interface_t *block_inf, void *ctx_in)
{
	// This runner function doesn't do anything!
}

static void *block_constant_io_value(block_interface_t *block_inf, void *ctx_in)
{
	block_constant_ctx_t *ctx = (block_constant_ctx_t *) ctx_in;
	return &ctx->value;
}

#endif // APP_BLOCK
