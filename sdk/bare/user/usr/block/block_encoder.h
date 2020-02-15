#ifndef BLOCK_ENCODER_H
#define BLOCK_ENCODER_H

#include "block_type.h"
#include <stdlib.h>

#define MAX_NUM_ENCODER_BLOCKS (8)

void block_encoder_init_all(void);
block_t *block_encoder_get(size_t idx);

#endif // BLOCK_ENCODER_H
