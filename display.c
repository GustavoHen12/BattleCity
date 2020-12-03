#include"display.h"

void check (bool test, const char *boot) {
    if(test) return;

    fprintf(stderr, "Não foi possível inicializar o %s\n", boot);
    exit(1);
}

int initConf () {
    //Inicia o allegro
    check(al_init(), "allegro");
    check(al_install_keyboard(), "keyboard");

    //Inicia o timer que ira gerar o evento a 60fps
    timer = al_create_timer(1.0 / 60.0);
    check(timer, "timer");

    //Inicia fila de eventos
    queue = al_create_event_queue();
    check(queue, "queue");

    //Seta as configurações da tela
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    //Inicia tela de DISPLAY_WIDHT x DISPLAY_HEIGH
    screen = al_create_display( DISPLAY_WIDHT,DISPLAY_HEIGH);
    check(screen, "display");

    //Inicia o buffer da tela
    buffer = al_create_bitmap(BUFFER_WIDHT, BUFFER_HEIGHT);
    check(buffer, "bitmap buffer");

    //Inicia font
    font = al_create_builtin_font();
    check(font, "font");

    //Inicia imagens
    check(al_init_image_addon(), "image addon");

    //Registra as fontes de eventos
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(screen));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    return 1;
}

/*
* "Retira" uma area do spritesheet
* Assim a partir de um posição (x, y) da imagem fonte 
* é retirado um sprite de tamanho "h" x "w"
*/
ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    //retira região da image
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    //verifica se a operação ocorreu de acordo e returna ponteiro para o sprite
    check(sprite, "sprite grab");
    return sprite;
}


void initSprites()
{
    sprites._sheet = al_load_bitmap("assets/spritesheet_battleCity.png");
    check(sprites._sheet, "spritesheet");

    sprites.tank[SPRITE_UP] = sprite_grab(2, 2, 30, 30);
    sprites.tank[SPRITE_RIGHT] = sprite_grab(2, 33, 30, 30);
    sprites.tank[SPRITE_DOWN] = sprite_grab(2, 64, 30, 30);
    sprites.tank[SPRITE_LEFT] = sprite_grab(2, 95, 30, 30);

}

int initDisplay (){
    //inicia as configurações do display
    initConf();

    //inicia sprites
    initSprites();

    return 1;
}

void startFPS(){
    //inicia a contagem do timer
    al_start_timer(timer);
}

void beforeDraw(){
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    // al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", x, y);
}

void drawDisplay(int x, int y, int type, int direction){
    //desenha o sprite "sprite" na tela na posicação (x, y)
    ALLEGRO_BITMAP* sprite;
    if(type == 0){
        sprite = sprites.tank[direction];
    }
    al_draw_bitmap(sprite, x, y, 0);
}

void showDraw(){
    al_set_target_backbuffer(screen);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_WIDHT, BUFFER_HEIGHT, 0, 0, DISPLAY_WIDHT, DISPLAY_HEIGH, 0);

    al_flip_display();
}

void closeDisplay(){
    //destroi objetos criados
    al_destroy_font(font);
    al_destroy_display(screen);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(screen);
}
