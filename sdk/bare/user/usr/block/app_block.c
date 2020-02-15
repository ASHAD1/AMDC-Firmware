#ifdef APP_BLOCK

#include "usr/block/app_block.h"
#include "usr/block/task_block_chain_runner.h"
#include "usr/block/block_type.h"
#include "usr/block/block_encoder.h"
#include "usr/block/block_print_int.h"
#include "sys/defines.h"

static void __stitch_encoder_printer(void);

static block_t *block_chain1[2] = {0};

void app_block_init(void)
{
	// ===========================
	// Add block chain runner task to system scheduler
	// ===========================

	task_block_chain_runner_init();

	// ===========================
	// Initialize all system blocks
	// ===========================

	block_encoder_init_all();
	block_print_int_init_all();

	// ===========================
	// Register block chains
	// ===========================

	__stitch_encoder_printer();
	task_block_chain_runner_register(block_chain1, ARRAY_SIZE(block_chain1));
}

// ========

static void __stitch_encoder_printer(void)
{
	// Allocate blocks to chain
	block_chain1[0] = block_encoder_get(0);
	block_chain1[1] = block_print_int_get(0);

	// Hook up inputs and outputs
	block_chain1[1]->inputs[0] = block_chain1[0]->outputs[0];

	// Configure block-specific info
	block_print_int_config_prepend(0, "enc steps: ");
}

#endif // APP_BLOCK
