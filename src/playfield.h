#ifndef TETRIS_PLAYFIELD_H
#define TETRIS_PLAYFIELD_H

#include "block.h"
#include "tetris.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 24
#define OFFSET_X 1
#define OFFSET_Y 4
#define BLOCK_SIZE 32

void clear_playfield();
void copy_block_to_playfield(block *b);
bool did_block_collide(block *b, int dx, int dy);
int check_for_lines();
void draw_playfield();

#endif