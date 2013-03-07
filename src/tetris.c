// gcc tetris.c block.c playfield.c allegro_framework.c -o tetris -lallegro-5.0.8-monolith-mt-debug -std=c99 -Wall

#include "tetris.h"
#include "playfield.h"
#include "block.h"

#define SCREEN_WIDTH ((FIELD_WIDTH+5) * BLOCK_SIZE)
#define SCREEN_HEIGHT ((FIELD_HEIGHT-2) * BLOCK_SIZE)

block current_block, next_block;
bool dead = false;
int score = 0, lines = 0, level = 0;
int fall_delay = 0, fall_speed = 30;
int key_delay = 0;
int counter = 0;

ALLEGRO_COLOR get_color(int color_index)
{
    ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);
    switch (color_index) {
        case 1: color = al_map_rgb(0, 255, 255); break;
        case 2: color = al_map_rgb(255, 255, 0); break;
        case 3: color = al_map_rgb(128, 0, 128); break;
        case 4: color = al_map_rgb(0, 255, 0); break;
        case 5: color = al_map_rgb(255, 0, 0); break;
        case 6: color = al_map_rgb(0, 0, 255); break;
        case 7: color = al_map_rgb(255, 128, 0); break;
    }
    return color;
}

void game_over()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    print_textf(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, white_color, TEXT_ALIGN_CENTER, "GAME OVER!");
    al_flip_display();
    wait_for_keypress();
}

void reset_game()
{
    dead = false;
    score = lines = level = 0;
    generate_block(&current_block);
    generate_next_block(&next_block);
    clear_playfield();
}

void logic()
{
    if (key_delay <= 0) {
        key_delay = 5;
        if (is_key_down(ALLEGRO_KEY_LEFT) && !did_block_collide(&current_block, -1, 0)) {
            current_block.x--;
        }
        else if (is_key_down(ALLEGRO_KEY_RIGHT) && !did_block_collide(&current_block, 1, 0)) {
            current_block.x++;
        }
        else if (is_key_down(ALLEGRO_KEY_DOWN) && !did_block_collide(&current_block, 0, 1)) {
            current_block.y++;
            score++;
        }
        else if (is_key_down(ALLEGRO_KEY_A)) {
            key_delay = 10;
            rotate_block_left(&current_block);
            if (did_block_collide(&current_block, 0, 0))
                rotate_block_right(&current_block);
        }
        else if (is_key_down(ALLEGRO_KEY_S)) {
            key_delay = 10;
            rotate_block_right(&current_block);
            if (did_block_collide(&current_block, 0, 0))
                rotate_block_left(&current_block);
        }
        else
            key_delay = 0;
    }
    key_delay--;
    
    if (fall_delay <= 0) {
        fall_delay = fall_speed;
        
        if (did_block_collide(&current_block, 0, 1)) {
            if (current_block.y == 0)
                dead = true;
        
            copy_block_to_playfield(&current_block);
            current_block = next_block;
            current_block.x = (FIELD_WIDTH / 2) - 2;
            current_block.y = 0;
            generate_next_block(&next_block);
        }
        else
            current_block.y++;
    }
    fall_delay--;
    
    // falling block speed increases every minute!
    counter++;
    if (counter > 3600) {
        counter = 0;
        level++;
        fall_speed = 30 - level;
    }
    
    // add score if we cleared any lines
    int cleared_lines = check_for_lines();
    lines += cleared_lines;
    switch (cleared_lines) {
        case 0: score += 0; break;
        case 1: score += level * 40 + 40; break;
        case 2: score += level * 100 + 100; break;
        case 3: score += level * 300 + 300; break;
        default: score += level * 1200 + 1200; break;
    }
    
    if (dead) {
        game_over();
        reset_game();
    }
    
    if (is_key_down(ALLEGRO_KEY_ESCAPE))
        quit();
}

void render()
{    
    // draw border
    ALLEGRO_COLOR border_color = al_map_rgb(0, 63, 128);
    al_draw_filled_rectangle(0, 0, BLOCK_SIZE, SCREEN_HEIGHT - BLOCK_SIZE, border_color);
    al_draw_filled_rectangle(SCREEN_WIDTH - (4 * BLOCK_SIZE), 0, SCREEN_WIDTH, SCREEN_HEIGHT - BLOCK_SIZE, border_color);
    al_draw_filled_rectangle(0, SCREEN_HEIGHT - (2 * BLOCK_SIZE), SCREEN_WIDTH, SCREEN_HEIGHT - BLOCK_SIZE, border_color);
    
    draw_playfield();
    draw_block(&current_block);
    draw_block(&next_block);
    
    // draw score
    print_textf(5, SCREEN_HEIGHT - 12, white_color, 0, "Score: %d  Lines: %d", score, lines);
}

int main() {
    init_framework(SCREEN_WIDTH, SCREEN_HEIGHT, false);
    reset_game();
    run_game_loop(logic, render);
    return 0;
}
