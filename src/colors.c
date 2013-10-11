#include "colors.h"

static ALLEGRO_COLOR color_index[MAX_COLORS];

void setup_color_index()
{
    color_index[0] = al_map_rgb(0, 0, 0);
    color_index[1] = al_map_rgb(0, 255, 255);
    color_index[2] = al_map_rgb(255, 255, 0);
    color_index[3] = al_map_rgb(128, 0, 128);
    color_index[4] = al_map_rgb(0, 255, 0);
    color_index[5] = al_map_rgb(255, 0, 0);
    color_index[6] = al_map_rgb(0, 0, 255);
    color_index[7] = al_map_rgb(255, 128, 0);
}

ALLEGRO_COLOR get_color_by_index(int index)
{
    assert(index >= 0 && index < MAX_COLORS);
    return color_index[index];
}
