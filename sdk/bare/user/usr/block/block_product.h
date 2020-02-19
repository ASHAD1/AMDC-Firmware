#ifndef BLOCK_PRODUCT_H
#define BLOCK_PRODUCT_H

#include <stdlib.h>
#include <usr/block/block_interface_type.h>

#define MAX_NUM_PRODUCT_BLOCKS (8)

void block_product_init_all(void);

block_interface_t *block_product_get_interface(size_t idx);
void *block_product_get_ctx(size_t idx);

void block_product_config_set_num_inputs(size_t idx, size_t num_inputs);

#endif // BLOCK_PRODUCT_H
