#ifndef INCLUDED_BLOCK_H
#define INCLUDED_BLOCK_H

#include "allegro_framework.h"

typedef struct {
    int type;   // what kind of tetromino (e.g. I, O, L...)
    int color;  // what color index the block has
    int x, y;   // location in the playfield
    int dir;    // used for rotating the block
    int data;   // data about the block shape (see block_data)
} block_type;

void generate_block_at(block_type *block, int x, int y);
void rotate_block_left(block_type *block);
void rotate_block_right(block_type *block);
void draw_block(block_type *block);

#endif
