#include "playfield.h"

int playfield[FIELD_HEIGHT * FIELD_WIDTH] = {0};

void clear_playfield()
{
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            playfield[(y * FIELD_WIDTH) + x] = 0;
        }
    }
}

void copy_block_to_playfield(block *b)
{
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (b->data & (1 << ((y * 4) + x)))
                playfield[((b->y + y) * FIELD_WIDTH) + b->x + x] = b->color;
}

bool did_block_collide(block *b, int dx, int dy)
{
    for (int y = 0; y < 4; y ++) {
        for (int x = 0; x < 4; x ++) {
            if ((((b->x + dx + x < 0) || (b->y + dy + y < 0) || 
                (b->x + dx + x >= FIELD_WIDTH) || (b->y + dy + y >= FIELD_HEIGHT)) ||
                (playfield[((b->y + dy + y) * FIELD_WIDTH) + b->x + dx + x] != 0)) && (b->data & (1 << ((y * 4) + x)))) {
                    return true;
            }
        }
    }
    
    return false;
}

int check_for_lines()
{
    int lines = 0;
    
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        int total = 0;
        
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (playfield[(y * FIELD_WIDTH) + x])
                total++;
        }
        
        if (total == FIELD_WIDTH) {
            for(int i = y; i > 0; i--)
                memcpy(&playfield[i * FIELD_WIDTH], &playfield[(i - 1) * FIELD_WIDTH], sizeof(int) * FIELD_WIDTH);
                
            memset(&playfield[0], 0, sizeof(int) * FIELD_WIDTH);
            lines++;
        }
    }
    
    return lines;
}

void draw_playfield()
{
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            int x0 = (x + OFFSET_X) * BLOCK_SIZE;
            int y0 = (y - OFFSET_Y) * BLOCK_SIZE;
            int x1 = x0 + BLOCK_SIZE;
            int y1 = y0 + BLOCK_SIZE;
            al_draw_filled_rectangle(x0, y0, x1, y1, get_color(playfield[y * FIELD_WIDTH + x]));
        }
    }
}
