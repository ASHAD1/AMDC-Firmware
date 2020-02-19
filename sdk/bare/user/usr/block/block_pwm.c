#ifdef APP_BLOCK

#include "usr/block/block_pwm.h"
#include <usr/block/block_interface_type.h>
#include "sys/debug.h"
#include "sys/defines.h"
#include "drv/pwm.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void block_pwm_runner(block_interface_t *block_inf, void *ctx_in);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_pwm_ctx_t {
	size_t channel;
} block_pwm_ctx_t;

static block_pwm_ctx_t block_pwm_ctxs[MAX_NUM_PWM_BLOCKS] = {0};
static block_interface_t block_pwm_interfaces[MAX_NUM_PWM_BLOCKS] = {0};

block_interface_t *block_pwm_get_interface(size_t idx)
{
	if (idx >= MAX_NUM_PWM_BLOCKS) {
		return NULL;
	}

	return &block_pwm_interfaces[idx];
}

void *block_pwm_get_ctx(size_t idx)
{
	if (idx >= MAX_NUM_PWM_BLOCKS) {
		return NULL;
	}

	return &block_pwm_ctxs[idx];
}

void block_pwm_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_PWM_BLOCKS; i++) {
		block_interface_t *b = &block_pwm_interfaces[i];

		// Configure inputs
		// ...
		b->num_inputs = 1;

		// Configure outputs
		// ...
		b->num_outputs = 0;

		// Configure run function
		// ...
		b->runner = block_pwm_runner;
	}
}

void block_pwm_config_set_channel(size_t idx, size_t channel)
{
	block_pwm_ctxs[idx].channel = channel;
}

// =================

static void block_pwm_runner(block_interface_t *block_inf, void *ctx_in)
{
	block_pwm_ctx_t *ctx = (block_pwm_ctx_t *) ctx_in;

	// Retrieve input as integer
	double duty = *((double *) block_inf->inputs[0].exec(block_inf, ctx));

	pwm_set_duty(ctx->channel, duty);
}

#endif // APP_BLOCK
