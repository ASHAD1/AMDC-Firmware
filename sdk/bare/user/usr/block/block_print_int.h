#ifndef BLOCK_PRINT_INT_H
#define BLOCK_PRINT_INT_H

#include <stdlib.h>
#include <usr/block/block_interface_type.h>

#define MAX_NUM_PRINT_INT_BLOCKS (8)

void block_print_int_init_all(void);

block_interface_t *block_print_int_get_interface(size_t idx);
void *block_print_int_get_ctx(size_t idx);

void block_print_int_config_prepend(size_t idx, char *msg);

#endif // BLOCK_PRINT_INT_H
