#ifndef BLOCK_TYPE_H
#define BLOCK_TYPE_H

#define BLOCK_MAX_NUM_INPUTS (8)
#define BLOCK_MAX_NUM_OUTPUTS (8)

typedef struct block_t block_t;

typedef void* (*block_io_t)(void *state);
typedef void (*block_run_t)(block_t *state);

typedef struct block_t {
	// Number of valid input functions for this block
	int num_inputs;

	// Array of input functions
	block_io_t inputs[BLOCK_MAX_NUM_INPUTS];

	// Number of valid output functions for this block
	int num_outputs;

	// Array of output functions
	block_io_t outputs[BLOCK_MAX_NUM_OUTPUTS];

	// Called to run block
	block_run_t runner;

	// Application-specific block state pointer
	void *state;
} block_t;

#endif // BLOCK_TYPE_H
