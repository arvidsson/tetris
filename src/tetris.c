// gcc src/tetris.c src/block.c src/playfield.c src/allegro_framework.c -o tetris -lallegro-5.0.8-monolith-mt-debug -std=c99 -Wall

#include "allegro_framework.h"
#include "playfield.h"
#include "block.h"

#define SCREEN_WIDTH ((FIELD_WIDTH+5) * BLOCK_SIZE)
#define SCREEN_HEIGHT ((FIELD_HEIGHT-2) * BLOCK_SIZE)

#define MAX_COLORS 8
ALLEGRO_COLOR color_index[MAX_COLORS];

block current_block;    // the block that is falling
block next_block;       // the next block coming down
bool dead = false;      // whether we are dead or not
int score = 0;          // accumulated score
int lines = 0;          // no of cleared lines
int level = 0;          // determines block fall speed, increases over time
int fall_delay = 0;     // the amount of time before moving a block down
int fall_speed = 30;    // block fall speed
int key_delay = 0;      // the amount of time before checking for next keypress
int counter = 0;        // used for counting

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

void game_over()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(get_default_font(), white_color, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 15, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
    al_draw_textf(get_default_font(), white_color, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 15, ALLEGRO_ALIGN_CENTER, "You got %d points", score);
    al_draw_textf(get_default_font(), white_color, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "You cleared %d lines", lines);
    al_draw_textf(get_default_font(), white_color, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 45, ALLEGRO_ALIGN_CENTER, "You reached level %d", level);
    al_flip_display();
    al_rest(2.0);
    wait_for_keypress();
}

void reset_game()
{
    dead = false;
    score = lines = level = 0;
    generate_block(&current_block);
    generate_block(&next_block);
    next_block.x = FIELD_WIDTH;
    next_block.y = 4;
    clear_playfield();
}

void update()
{
    //we check input if enough time has passed since the last keypress
    if (key_delay <= 0) {
        key_delay = 5;
        
        if (is_key_down(ALLEGRO_KEY_LEFT) && !is_collision(&current_block, -1, 0)) {
            current_block.x--;
        }
        else if (is_key_down(ALLEGRO_KEY_RIGHT) && !is_collision(&current_block, 1, 0)) {
            current_block.x++;
        }
        else if (is_key_down(ALLEGRO_KEY_DOWN) && !is_collision(&current_block, 0, 1)) {
            current_block.y++;
            score++;
        }
        else if (is_key_down(ALLEGRO_KEY_A)) {
            key_delay = 10;
            rotate_block_left(&current_block);
            if (is_collision(&current_block, 0, 0))
                rotate_block_right(&current_block);
        }
        else if (is_key_down(ALLEGRO_KEY_S)) {
            key_delay = 10;
            rotate_block_right(&current_block);
            if (is_collision(&current_block, 0, 0))
                rotate_block_left(&current_block);
        }
        else if (is_key_down(ALLEGRO_KEY_SPACE)) {
            key_delay = 20;
            while (!is_collision(&current_block, 0, 1))
                current_block.y++;
        }
        else {
            key_delay = 0;
        }
    }
    key_delay--;
    
    // if it's time, let the block fall!
    if (fall_delay <= 0) {
        fall_delay = fall_speed;
        
        if (is_collision(&current_block, 0, 1)) {
            if (current_block.y == 0) {
                dead = true;
            }
        
            copy_block_to_playfield(&current_block);
            
            // make the next block the current one
            current_block = next_block;
            current_block.x = (FIELD_WIDTH / 2) - 2;
            current_block.y = 0;
            
            // generate a new next block
            generate_block(&next_block);
            next_block.x = FIELD_WIDTH;
            next_block.y = 4;
        }
        else {
            current_block.y++;
        }
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
    
    if (is_key_down(ALLEGRO_KEY_ESCAPE)) {
        quit();
    }
}

void draw()
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
    al_draw_textf(get_default_font(), white_color, 5, SCREEN_HEIGHT - 12, ALLEGRO_ALIGN_LEFT, "Score: %d  Lines: %d", score, lines);
}

int main() {
    init_framework("tetris", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    setup_color_index();
    reset_game();
    run_game_loop(update, draw);
    return 0;
}
