#ifndef BLOCK_PWM_H
#define BLOCK_PWM_H

#include <stdlib.h>
#include <usr/block/block_interface_type.h>

#define MAX_NUM_PWM_BLOCKS (8)

void block_pwm_init_all(void);

block_interface_t *block_pwm_get_interface(size_t idx);
void *block_pwm_get_ctx(size_t idx);

void block_pwm_config_set_channel(size_t idx, size_t channel);

#endif // BLOCK_PWM_H
