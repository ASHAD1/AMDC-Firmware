#ifdef APP_BLOCK

#include "usr/block/block_encoder.h"
#include "usr/block/block_interface_type.h"
#include "drv/encoder.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_encoder_io_steps(block_interface_t *block_inf, void *ctx);
static void *block_encoder_io_position(block_interface_t *block_inf, void *ctx);
static void block_encoder_runner(block_interface_t *block_inf, void *ctx);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_encoder_ctx_t {
	uint32_t position;
	int32_t steps;
} block_encoder_ctx_t;

static block_encoder_ctx_t block_encoder_ctxs[MAX_NUM_ENCODER_BLOCKS] = {0};
static block_interface_t block_encoder_interfaces[MAX_NUM_ENCODER_BLOCKS] = {0};

block_interface_t *block_encoder_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_ENCODER_BLOCKS) {
		return NULL;
	}

	return &block_encoder_interfaces[idx];
}

void *block_encoder_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_ENCODER_BLOCKS) {
		return NULL;
	}

	return &block_encoder_ctxs[idx];
}

void block_encoder_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_ENCODER_BLOCKS; i++) {
		block_interface_t *b = &block_encoder_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 2;
		b->outputs[0].exec = block_encoder_io_steps;
		b->outputs[0].ctx = &block_encoder_ctxs[i];
		b->outputs[1].exec = block_encoder_io_position;
		b->outputs[1].ctx = &block_encoder_ctxs[i];

		// Configure run function
		// ...
		b->runner = block_encoder_runner;
	}
}

// =================

static void block_encoder_runner(block_interface_t *block_inf, void *ctx_in)
{
	block_encoder_ctx_t *ctx = (block_encoder_ctx_t *) ctx_in;

	// Step 1. Gather necessary inputs by calling input functions
	// ...
	// ...


	// Step 2. Run meat of block (controller code, etc)
	// ...
	// ...
	// ...


	// Step 3. Generic outputs and store in context
    // ...
	// ...
	encoder_get_steps(&ctx->steps);
	encoder_get_position(&ctx->position);
}

static void *block_encoder_io_steps(block_interface_t *block_inf, void *ctx_in)
{
	block_encoder_ctx_t *ctx = (block_encoder_ctx_t *) ctx_in;
	return &ctx->steps;
}

static void *block_encoder_io_position(block_interface_t *block_inf, void *ctx_in)
{
	block_encoder_ctx_t *ctx = (block_encoder_ctx_t *) ctx_in;
	return &ctx->position;
}

#endif // APP_BLOCK
