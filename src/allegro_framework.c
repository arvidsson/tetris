#include "allegro_framework.h"
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FILE *logfile = NULL;
ALLEGRO_FONT *default_font = NULL;

bool done = false;
bool keys[ALLEGRO_KEY_MAX] = { false };
int mouse_x = 0, mouse_y = 0;
bool mouse_buttons[MAX_MOUSE_BUTTONS] = { false };

ALLEGRO_COLOR black_color;
ALLEGRO_COLOR white_color;

void write_logfile(int log_level, const char *format, ...)
{
    char buffer[4096];
    
    if (log_level == LOG_WARNING)
        al_fputs(logfile, "WARNING: ");
    else if (log_level == LOG_ERROR)
        al_fputs(logfile, "ERROR: ");
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    al_fputs(logfile, buffer);
    al_fputs(logfile, "\n");
    
    if (log_level == LOG_ERROR)
        exit(1);
}

void init_framework(int display_width, int display_height, bool fullscreen)
{
    logfile = al_fopen("log.txt", "w");
    
    if (!al_init())
        log_error("Failed to initialize allegro");
    
    if (!al_install_keyboard())
        log_error("Failed to install keyboard");
    
    if (!al_install_mouse())
        log_error("Failed to install mouse");
    
    if (!al_init_primitives_addon())
        log_error("Failed to init primitives addon");
    
    if (!al_init_image_addon())
        log_error("Failed to init image addon");
    
    al_init_font_addon();
    default_font = al_create_builtin_font();
    if (!default_font)
        log_error("Failed to create builtin font");
    
    event_queue = al_create_event_queue();
    if (!event_queue)
        log_error("Failed to create event queue");
    
    if (fullscreen)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    else
        al_set_new_display_flags(ALLEGRO_WINDOWED);
    
    display = al_create_display(display_width, display_height);
    if (!display)
        log_error("Failed to create display @ %dx%d", display_width, display_height);
    
    timer = al_create_timer(1.0 / 60);
    if (!timer)
        log_error("Failed to create timer");
    
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    black_color = al_map_rgb(0, 0, 0);
    white_color = al_map_rgb(255, 255, 255);
    
    srand(time(NULL));
    atexit(destroy_framework);
}

void destroy_framework()
{
    if (default_font)
        al_destroy_font(default_font);
    if (timer)
        al_destroy_timer(timer);
    if (display)
        al_destroy_display(display);
    if (event_queue)
        al_destroy_event_queue(event_queue);
    if (logfile)
        al_fclose(logfile);
}

void run_game_loop(void (*logic_callback)(), void (*render_callback)())
{
    bool redraw = true;
    al_start_timer(timer);
    void (*logic_proc)() = logic_callback;
    void (*render_proc)() = render_callback;
    
    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                logic_proc();
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                keys[event.keyboard.keycode] = true;
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                keys[event.keyboard.keycode] = false;
                break;
            
            case ALLEGRO_EVENT_MOUSE_AXES:
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
                break;
            
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                mouse_buttons[event.mouse.button] = true;
                break;
            
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                mouse_buttons[event.mouse.button] = false;
                break;
        }
        
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_set_target_backbuffer(display);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            render_proc();
            al_flip_display();
        }
    }
}

void quit()
{
    done = true;
}

int get_window_width()
{
    return al_get_display_width(display);
}

int get_window_height()
{
    return al_get_display_height(display);
}

bool is_key_down(int keycode)
{
    return keys[keycode];
}

int get_mouse_x()
{
    return mouse_x;
}

int get_mouse_y()
{
    return mouse_y;
}

bool is_mouse_button_down(int mouse_button)
{
    return mouse_buttons[mouse_button];
}

int wait_for_keypress()
{
    ALLEGRO_EVENT event;
    do
        al_wait_for_event(event_queue, &event);
    while (event.type != ALLEGRO_EVENT_KEY_DOWN);
    return event.keyboard.keycode;
}

void print_textf(float x, float y, ALLEGRO_COLOR color, int flags, const char *format, ...)
{
    char buffer[4096];
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    al_draw_text(default_font, color, x, y, flags, buffer);
}

int get_random_int(int max, int min)
{
    return min + (rand() % (int)(max - min + 1));
}