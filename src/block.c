#include "block.h"
#include "playfield.h"

#define MAX_COLORS 8
extern ALLEGRO_COLOR color_index[MAX_COLORS];

// each block's 4 possible rotations are stored in an int
int block_data[7][4] = {
    {0x0F00, 0x2222, 0x00F0, 0x4444}, // I
    {0xCC00, 0xCC00, 0xCC00, 0xCC00}, // O
    {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
    {0x6C00, 0x4620, 0x06C0, 0x8C40}, // S
    {0xC600, 0x2640, 0x0C60, 0x4C80}, // Z
    {0x8E00, 0x6440, 0x0E20, 0x44C0}, // J
    {0x2E00, 0x4460, 0x0E80, 0xC440}  // L
};

void generate_block(block *b)
{
    int i = get_random_int(0, 6);
    b->type = i;
    b->x = (FIELD_WIDTH / 2) - 2;
    b->y = 0;
    b->dir = 0;
    b->color = i + 1;
    b->data = block_data[i][0];
}

void rotate_block_left(block *b)
{
    b->dir--;
    if (b->dir < 0) {
        b->dir = 3;
    }
    b->data = block_data[b->type][b->dir];
}

void rotate_block_right(block *b)
{
    b->dir++;
    if (b->dir > 3) {
        b->dir = 0;
    }
    b->data = block_data[b->type][b->dir];
}

void draw_block(block *b)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (b->data & (1 << ((y * 4) + x))) {
                int x0 = (b->x + OFFSET_X + x) * BLOCK_SIZE;
                int y0 = (b->y - OFFSET_Y + y) * BLOCK_SIZE;
                int x1 = x0 + BLOCK_SIZE;
                int y1 = y0 + BLOCK_SIZE;
                al_draw_filled_rectangle(x0, y0, x1, y1, color_index[b->color]);
            }
        }
    }
}
