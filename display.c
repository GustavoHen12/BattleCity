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

    //Inicia font
    font = al_create_builtin_font();
    check(font, "fonte");

    //Inicia imagens
    check(al_init_image_addon(), "image addon");

    //inicia primitivos
    check(al_init_primitives_addon(), "primitivos");

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
    //sprites._sheet = al_load_bitmap("resources/spritesheet_battleCity.png");
    sprites._sheet = al_load_bitmap("resources/sprite.bmp");
    check(sprites._sheet, "spritesheet");

    sprites.tank[SPRITE_UP] = sprite_grab(2, 2, 30, 30);
    sprites.tank[SPRITE_RIGHT] = sprite_grab(2, 33, 30, 30);
    sprites.tank[SPRITE_DOWN] = sprite_grab(2, 64, 30, 30);
    sprites.tank[SPRITE_LEFT] = sprite_grab(2, 95, 30, 30);

    sprites.enemies[SPRITE_UP] = sprite_grab(2, 126, 30, 32);
    sprites.enemies[SPRITE_RIGHT] = sprite_grab(2, 157, 30, 32);
    sprites.enemies[SPRITE_DOWN] = sprite_grab(2, 188, 30, 32);
    sprites.enemies[SPRITE_LEFT] = sprite_grab(2, 219, 30, 32);

    sprites.shots[SPRITE_UP] = sprite_grab(0, 350, 8, 9);
    sprites.shots[SPRITE_RIGHT] = sprite_grab(8, 350, 8, 9);
    sprites.shots[SPRITE_DOWN] = sprite_grab(16, 350, 8, 9);
    sprites.shots[SPRITE_LEFT] = sprite_grab(24, 350, 8, 9);

    sprites.explosion[0] = sprite_grab(255, 64, 32, 35);
    sprites.explosion[1] = sprite_grab(290, 64, 32, 45);
    sprites.explosion[2] = sprite_grab(320, 64, 45, 45);
    
    sprites.block[0] = sprite_grab(96, 264, 16, BLOCK_H);
    sprites.block[1] = sprite_grab(184, 264, 16, BLOCK_H);
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

void drawDisplay(int x, int y, int type, int direction){
    //desenha o sprite "sprite" na tela na posicação (x, y)
    ALLEGRO_BITMAP* sprite;
    if(type == SPRITE_TANK){
        sprite = sprites.tank[direction];
    }
    if(type == SPRITE_ENEMIES){
        sprite = sprites.enemies[direction];
    }
    if(type == SPRITE_SHOT){
        sprite = sprites.shots[direction];
    }
    if(type == SPRITE_BLOCK){
        sprite = sprites.block[direction];
    }

    al_draw_bitmap(sprite, x, y, 0);
}

void beforeDraw(){
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void showDraw(){
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

// ------------ Efeitos -----------

//TODO: REFATORAR OU COLOCAR CRÉDITOS
void fx_init()
{
    for(int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(int x, int y)
{
    for(int i = 0; i < FX_N; i++)
    {
        if(fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].used = true;
        return;
    }
}

void fx_update()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        fx[i].frame++;

        if(fx[i].frame == (EXPLOSION_FRAMES * 8))
            fx[i].used = false;
    }
}

void fx_draw()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 8;
        ALLEGRO_BITMAP *bmp =  sprites.explosion[frame_display];

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}

