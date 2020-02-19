#ifndef BLOCK_CONSTANT_H
#define BLOCK_CONSTANT_H

#include <stdlib.h>
#include <usr/block/block_interface_type.h>

#define MAX_NUM_CONSTANT_BLOCKS (8)

void block_constant_init_all(void);

block_interface_t *block_constant_get_interface(size_t idx);
void *block_constant_get_ctx(size_t idx);

void block_constant_config_set_value(size_t idx, double value);

#endif // BLOCK_CONSTANT_H
