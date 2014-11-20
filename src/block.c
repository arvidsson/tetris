#include "block.h"
#include "playfield.h"
#include "colors.h"

// each block's 4 possible rotations are stored in an int
static int block_data[7][4] = {
    {0x0F00, 0x2222, 0x00F0, 0x4444}, // I
    {0xCC00, 0xCC00, 0xCC00, 0xCC00}, // O
    {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
    {0x6C00, 0x4620, 0x06C0, 0x8C40}, // S
    {0xC600, 0x2640, 0x0C60, 0x4C80}, // Z
    {0x8E00, 0x6440, 0x0E20, 0x44C0}, // J
    {0x2E00, 0x4460, 0x0E80, 0xC440}  // L
};

void generate_block_at(Block *block, int x, int y)
{
    int i = get_random_int(0, 6);
    block->type = i;
    block->x = x;
    block->y = y;
    block->dir = 0;
    block->color = i + 1;
    block->data = block_data[i][0];
}

void rotate_block_left(Block *block)
{
    block->dir--;
    if (block->dir < 0) {
        block->dir = 3;
    }
    block->data = block_data[block->type][block->dir];
}

void rotate_block_right(Block *block)
{
    block->dir++;
    if (block->dir > 3) {
        block->dir = 0;
    }
    block->data = block_data[block->type][block->dir];
}

void draw_block(Block *block)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (block->data & (1 << ((y * 4) + x))) {
                int x0 = (block->x + OFFSET_X + x) * BLOCK_SIZE;
                int y0 = (block->y - OFFSET_Y + y) * BLOCK_SIZE;
                int x1 = x0 + BLOCK_SIZE;
                int y1 = y0 + BLOCK_SIZE;
                al_draw_filled_rectangle(x0, y0, x1, y1, get_color_by_index(block->color));
            }
        }
    }
}
