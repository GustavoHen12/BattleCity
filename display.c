#include"display.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int initDisplay () {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    disp = al_create_display( DISPLAY_WIDHT,DISPLAY_HEIGH);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_WIDHT, BUFFER_HEIGHT);
    must_init(buffer, "bitmap buffer");

    font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    return 1;
}

void startFPS(){
    al_start_timer(timer);
}

void drawDisplay(int x, int y, ALLEGRO_BITMAP *sprite){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", x, y);
    al_draw_bitmap(sprite, x, y, 0);
}

void closeDisplay(){
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(disp);
}

void beforeDraw(){
    al_set_target_bitmap(buffer);
}

void showDraw(){
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_WIDHT, BUFFER_HEIGHT, 0, 0, DISPLAY_WIDHT, DISPLAY_HEIGH, 0);

    al_flip_display();
}

ALLEGRO_BITMAP *loadSprite(char *filename){
    ALLEGRO_BITMAP* sprite = al_load_bitmap(filename);
    must_init(sprite, "sprite");

    return sprite;
}

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("assets/spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.xWing = sprite_grab(495, 0, xWing_H, xWing_W);

    sprites.tie1 = sprite_grab(103, 300, tie_H, tie_W);
    sprites.tie2 = sprite_grab(325, 220, tie_H, tie_W);
}
