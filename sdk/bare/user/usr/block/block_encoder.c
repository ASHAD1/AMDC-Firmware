#ifdef APP_BLOCK

#include "usr/block/block_encoder.h"
#include "usr/block/block_type.h"
#include "drv/encoder.h"
#include <stdlib.h>
#include <stdint.h>

// Block I/O and run functions
// NOTE: Must adhere to the interface defined in block_type.h
static void *block_encoder_io_steps(void *arg);
static void *block_encoder_io_position(void *arg);
static void block_encoder_runner(block_t *block);

// Block-specific state information
// NOTE: this is maintained between block executions
typedef struct block_encoder_state_t {
	uint32_t position;
	int32_t steps;
} block_encoder_state_t;

static block_encoder_state_t block_encoder_state[MAX_NUM_ENCODER_BLOCKS] = {0};
static block_t block_encoder_blocks[MAX_NUM_ENCODER_BLOCKS] = {0};

block_t *block_encoder_get(size_t idx)
{
	if (idx >= MAX_NUM_ENCODER_BLOCKS) {
		return NULL;
	}

	return &block_encoder_blocks[idx];
}

void block_encoder_init_all(void)
{
	for (size_t i = 0; i < MAX_NUM_ENCODER_BLOCKS; i++) {
		block_t *b = block_encoder_get(i);

		// Configure inputs
		// ...
		b->num_inputs = 0;

		// Configure outputs
		// ...
		b->num_outputs = 2;
		b->outputs[0] = block_encoder_io_steps;
		b->outputs[1] = block_encoder_io_position;

		// Configure argument pointer
		// ...
		b->state = &block_encoder_state[i];

		// Configure run function
		// ...
		b->runner = block_encoder_runner;
	}
}

// =================

static void block_encoder_runner(block_t *block)
{
	block_encoder_state_t *state = (block_encoder_state_t *) &block->state;

	// Step 1. Gather necessary inputs by calling input functions
	// ...
	// ...


	// Step 2. Run meat of block (controller code, etc)
	// ...
	// ...
	// ...


	// Step 3. Generic outputs and store in state struct
    // ...
	// ...
	encoder_get_steps(&state->steps);
	encoder_get_position(&state->position);
}

static void *block_encoder_io_steps(void *arg)
{
	block_encoder_state_t *state = (block_encoder_state_t *) arg;
	return &state->steps;
}

static void *block_encoder_io_position(void *arg)
{
	block_encoder_state_t *state = (block_encoder_state_t *) arg;
	return &state->position;
}

#endif // APP_BLOCK
