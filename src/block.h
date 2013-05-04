#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H

#include "allegro_framework.h"

typedef struct {
    int type;   // what kind of tetromino (e.g. I, O, L...)
    int color;  // what color index the block has
    int x, y;   // location in the playfield
    int dir;    // used for rotating the block
    int data;   // data about the block shape (see block_data)
} block;

void generate_block(block *b);
void rotate_block_left(block *b);
void rotate_block_right(block *b);
void draw_block(block* b);

#endif
