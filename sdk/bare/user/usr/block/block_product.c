#ifdef APP_BLOCK

#include "usr/block/block_product.h"
#include <usr/block/block_interface_type.h>
#include "sys/debug.h"
#include "sys/defines.h"
#include "drv/pwm.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_product_io_output(block_interface_t *block_inf, void *ctx);
static void block_product_runner(block_interface_t *block_inf, void *ctx_in);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_product_ctx_t {
	double output;
} block_product_ctx_t;

static block_product_ctx_t block_product_ctxs[MAX_NUM_PRODUCT_BLOCKS] = {0};
static block_interface_t block_product_interfaces[MAX_NUM_PRODUCT_BLOCKS] = {0};

block_interface_t *block_product_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_PRODUCT_BLOCKS) {
		return NULL;
	}

	return &block_product_interfaces[idx];
}

void *block_product_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_PRODUCT_BLOCKS) {
		return NULL;
	}

	return &block_product_ctxs[idx];
}

void block_product_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_PRODUCT_BLOCKS; i++) {
		block_interface_t *b = &block_product_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 1;
		b->outputs[0].exec = block_product_io_output;
		b->outputs[0].ctx = &block_product_ctxs[i];

		// Configure run function
		// ...
		b->runner = block_product_runner;
	}
}

void block_product_config_set_num_inputs(size_t idx, size_t num_inputs)
{
	block_product_interfaces[idx].num_inputs = num_inputs;
}

// =================

static void block_product_runner(block_interface_t *block_inf, void *ctx_in)
{
	block_product_ctx_t *ctx = (block_product_ctx_t *) ctx_in;

	if (block_inf->num_inputs > 0) {
		// Initialize output
		double out = *(double *)(block_inf->inputs[0].exec(block_inf, ctx));

		for (size_t i = 1; i < block_inf->num_inputs; i++) {
			out *= *(double *)(block_inf->inputs[i].exec(block_inf, ctx));
		}

		ctx->output = out;
	} else {
		// No inputs set up!
		ctx->output = 0.0;
	}
}

static void *block_product_io_output(block_interface_t *block_inf, void *ctx_in)
{
	block_product_ctx_t *ctx = (block_product_ctx_t *) ctx_in;
	return &ctx->output;
}

#endif // APP_BLOCK
