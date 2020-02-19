#ifndef BLOCK_INTERFACE_TYPE_H
#define BLOCK_INTERFACE_TYPE_H

#define BLOCK_INTERFACE_MAX_NUM_INPUTS (8)
#define BLOCK_INTERFACE_MAX_NUM_OUTPUTS (8)

typedef struct block_interface_t block_interface_t;

typedef void* (*block_io_t)(block_interface_t *block, void *ctx);
typedef void (*block_run_t)(block_interface_t *block, void *ctx);

typedef struct block_io_func_t {
	block_io_t exec;
	void *ctx;
} block_io_func_t;

typedef struct block_interface_t {
	// Number of valid input functions for this block
	int num_inputs;

	// Array of input functions
	block_io_func_t inputs[BLOCK_INTERFACE_MAX_NUM_INPUTS];

	// Number of valid output functions for this block
	int num_outputs;

	// Array of output functions
	block_io_func_t outputs[BLOCK_INTERFACE_MAX_NUM_OUTPUTS];

	// Called to run block
	block_run_t runner;
} block_interface_t;

typedef struct block_t {
	block_interface_t *interface;
	void *ctx;
} block_t;

#endif // BLOCK_INTERFACE_TYPE_H
