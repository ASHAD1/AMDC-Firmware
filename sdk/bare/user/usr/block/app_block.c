
#ifdef APP_BLOCK

#include "usr/block/app_block.h"
#include "usr/block/task_block_chain_runner.h"
#include "usr/block/block_interface_type.h"
#include "usr/block/block_pwm.h"
#include "usr/block/block_adc.h"
#include "usr/block/block_encoder.h"
#include "usr/block/block_constant.h"
#include "usr/block/block_product.h"
#include "usr/block/block_print_int.h"
#include "sys/defines.h"

static void __stitch_encoder_printer(void);
static void __stitch_pwm(void);

static block_t *block_chain1[2] = {0};
static block_t *block_chain2[5] = {0};

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
	block_adc_init_all();
	block_pwm_init_all();
	block_constant_init_all();
	block_product_init_all();
	block_print_int_init_all();

	// ===========================
	// Register block chains
	// ===========================

	__stitch_encoder_printer();
	__stitch_pwm();

	task_block_chain_runner_register(block_chain1, ARRAY_SIZE(block_chain1));
	task_block_chain_runner_register(block_chain2, ARRAY_SIZE(block_chain2));
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

static void __stitch_pwm(void)
{
	block_t *constant   = block_chain2[0];
	block_t *adc        = block_chain2[1];
	block_t *product    = block_chain2[2];
	block_t *pwm1       = block_chain2[3];
	block_t *pwm2       = block_chain2[4];

	// Allocate blocks to chain which uses:
	// - 1 [const]
	// - 1 [product]
	// - 1 [adc]
	// - 2 [pwm]
	constant->interface = block_constant_get_interface(0);
	constant->ctx       = block_constant_get_ctx(0);
	product->interface  = block_product_get_interface(0);
	product->ctx        = block_product_get_ctx(0);
	adc->interface      = block_adc_get_interface(0);
	adc->ctx            = block_adc_get_ctx(0);
	pwm1->interface     = block_pwm_get_interface(0);
	pwm1->ctx           = block_pwm_get_ctx(0);
	pwm2->interface     = block_pwm_get_interface(1);
	pwm2->ctx           = block_pwm_get_ctx(1);

	// Hook up inputs to outputs
	pwm1->interface->inputs[0].exec = constant->interface->outputs[0].exec;
	pwm1->interface->inputs[0].ctx  = constant->interface->outputs[0].ctx;
	product->interface->inputs[0].exec = constant->interface->outputs[0].exec;
	product->interface->inputs[0].ctx  = constant->interface->outputs[0].ctx;
	product->interface->inputs[1].exec = adc->interface->outputs[0].exec;
	product->interface->inputs[1].ctx  = adc->interface->outputs[0].ctx;
	pwm2->interface->inputs[0].exec = product->interface->outputs[0].exec;
	pwm2->interface->inputs[0].ctx  = product->interface->outputs[0].ctx;

	// Configure block-specific info
	block_constant_config_set_value(0, 0.5);
	block_pwm_config_set_channel(0, 0); // output PWM1 on channel 0
	block_pwm_config_set_channel(1, 1); // output PWM2 on channel 1
	block_adc_config_set_channel(0, 0); // input ADC on channel 0
	block_product_config_set_num_inputs(0, 2); // multiply 2 numbers
}

#endif // APP_BLOCK
