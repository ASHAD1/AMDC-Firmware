#ifdef APP_BLOCK

#include "usr/block/block_adc.h"
#include "usr/block/block_interface_type.h"
#include "drv/analog.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_adc_io_voltage(block_interface_t *block_inf, void *ctx);
static void block_adc_runner(block_interface_t *block_inf, void *ctx);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_adc_ctx_t {
	size_t channel;
	double voltage;
} block_adc_ctx_t;

static block_adc_ctx_t block_adc_ctxs[MAX_NUM_ADC_BLOCKS] = {0};
static block_interface_t block_adc_interfaces[MAX_NUM_ADC_BLOCKS] = {0};

block_interface_t *block_adc_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_ADC_BLOCKS) {
		return NULL;
	}

	return &block_adc_interfaces[idx];
}

void *block_adc_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_ADC_BLOCKS) {
		return NULL;
	}

	return &block_adc_ctxs[idx];
}

void block_adc_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_ADC_BLOCKS; i++) {
		block_interface_t *b = &block_adc_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 1;
		b->outputs[0].exec = block_adc_io_voltage;
		b->outputs[0].ctx = &block_adc_ctxs[i];

		// Configure run function
		// ...
		b->runner = block_adc_runner;
	}
}

void block_adc_config_set_channel(size_t idx, size_t channel)
{
	block_adc_ctxs[idx].channel = channel;
}

// =================

static void block_adc_runner(block_interface_t *block_inf, void *ctx_in)
{
	block_adc_ctx_t *ctx = (block_adc_ctx_t *) ctx_in;

	float voltage_f;
	analog_getf(ctx->channel - 1, &voltage_f);
	ctx->voltage = (double) voltage_f;
}

static void *block_adc_io_voltage(block_interface_t *block_inf, void *ctx_in)
{
	block_adc_ctx_t *ctx = (block_adc_ctx_t *) ctx_in;
	return &ctx->voltage;
}

#endif // APP_BLOCK
