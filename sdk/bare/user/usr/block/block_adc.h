#ifndef BLOCK_ADC_H
#define BLOCK_ADC_H

#include <stdlib.h>
#include "usr/block/block_interface_type.h"

#define MAX_NUM_ADC_BLOCKS (8)

void block_adc_init_all(void);

block_interface_t *block_adc_get_interface(size_t idx);
void *block_adc_get_ctx(size_t idx);

void block_adc_config_set_channel(size_t idx, size_t channel);

#endif // BLOCK_ADC_H
