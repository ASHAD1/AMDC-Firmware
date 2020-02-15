#ifndef BLOCK_PRINT_INT_H
#define BLOCK_PRINT_INT_H

#include "block_type.h"
#include <stdlib.h>

#define MAX_NUM_PRINT_INT_BLOCKS (8)

void block_print_int_init_all(void);
block_t *block_print_int_get(size_t idx);

void block_print_int_config_prepend(size_t idx, char *msg);

#endif // BLOCK_PRINT_INT_H
