#ifndef BLOCK_ENCODER_H
#define BLOCK_ENCODER_H

#include <stdlib.h>
#include "usr/block/block_interface_type.h"

#define MAX_NUM_ENCODER_BLOCKS (8)

void block_encoder_init_all(void);

block_interface_t *block_encoder_get_interface(size_t idx);
void *block_encoder_get_ctx(size_t idx);

#endif // BLOCK_ENCODER_H
