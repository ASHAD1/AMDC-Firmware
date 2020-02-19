#ifndef BLOCK_CONST_H
#define BLOCK_CONST_H

#include <stdlib.h>
#include <usr/block/block_interface_type.h>

#define MAX_NUM_CONST_BLOCKS (8)

void block_const_init_all(void);

block_interface_t *block_const_get_interface(size_t idx);
void *block_const_get_ctx(size_t idx);

void block_const_config_set_value(size_t idx, double value);

#endif // BLOCK_CONST_H
