
#ifdef APP_BLOCK

#include "usr/block/app_block.h"
#include "usr/block/task_block_chain_runner.h"
#include "usr/block/block_interface_type.h"
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
	block_t *encoder   = block_chain1[0];
	block_t *print_int = block_chain1[1];

	// Allocate blocks to chain (uses 1 [encoder] + 1 [print_int])
	encoder->interface   = block_encoder_get_interface(0);
	encoder->ctx         = block_encoder_get_ctx(0);
	print_int->interface = block_print_int_get_interface(0);
	print_int->ctx       = block_print_int_get_ctx(0);

	// Hook up inputs and outputs
	print_int->interface->inputs[0].exec = encoder->interface->outputs[0].exec;
	print_int->interface->inputs[0].ctx  = encoder->interface->outputs[0].ctx;

	// Configure block-specific info
	block_print_int_config_prepend(0, "enc steps: ");
}

#endif // APP_BLOCK
