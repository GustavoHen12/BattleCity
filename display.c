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

    //inicia sons
    check(al_install_audio(), "audio");
    check(al_init_acodec_addon(), "audio codecs");
    check(al_reserve_samples(16), "reserve samples");

    //inicia spritesheet
    _sheet = al_load_bitmap("resources/spritesheet_battleCity.png");
    //sprites._sheet = al_load_bitmap("resources/sprite.bmp");
    check(_sheet, "spritesheet");

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
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(_sheet, x, y, w, h);
    //verifica se a operação ocorreu de acordo e returna ponteiro para o sprite
    check(sprite, "sprite grab");
    return sprite;
}

void initSprites()
{
    sprites.tank[SPRITE_UP] = sprite_grab(2, 2, TANK_W, TANK_H);
    sprites.tank[SPRITE_RIGHT] = sprite_grab(2, 34, TANK_W, TANK_H);
    sprites.tank[SPRITE_DOWN] = sprite_grab(2, 65, TANK_W, TANK_H);
    sprites.tank[SPRITE_LEFT] = sprite_grab(2, 98, TANK_W, TANK_H);

    sprites.enemies[SPRITE_UP] = sprite_grab(2, 426, ENEMIES_W, ENEMIES_H);
    sprites.enemies[SPRITE_RIGHT] = sprite_grab(2, 459, ENEMIES_W, ENEMIES_H);
    sprites.enemies[SPRITE_DOWN] = sprite_grab(2, 489, ENEMIES_W, ENEMIES_H);
    sprites.enemies[SPRITE_LEFT] = sprite_grab(2, 522, ENEMIES_W, ENEMIES_H);

    sprites.shots[SPRITE_UP] = sprite_grab(0, 350, SHOT_W, SHOT_H);
    sprites.shots[SPRITE_RIGHT] = sprite_grab(8, 352, SHOT_W, SHOT_H);
    sprites.shots[SPRITE_DOWN] = sprite_grab(16, 350, SHOT_W, SHOT_H);
    sprites.shots[SPRITE_LEFT] = sprite_grab(24, 352, SHOT_W, SHOT_H);
    
    sprites.block[0] = sprite_grab(96, 264, BLOCK_W, BLOCK_H);
    sprites.block[1] = sprite_grab(184, 264, BLOCK_W, BLOCK_H);
    sprites.stone = sprite_grab(0, 272, BLOCK_W, BLOCK_H*2);
    sprites.bush = sprite_grab(0, 304, BLOCK_W, BLOCK_H*2);

    sprites.explosion[0] = sprite_grab(255, 64, 32, 35);
    sprites.explosion[1] = sprite_grab(290, 64, 32, 45);
    sprites.explosion[2] = sprite_grab(320, 64, 45, 45);

    sprites.creation[0] = sprite_grab(258, 34, 30, 30);
    sprites.creation[1] = sprite_grab(288, 34, 30, 30);
    sprites.creation[2] = sprite_grab(318, 34, 30, 30);
    sprites.creation[3] = sprite_grab(352, 34, 30, 30);
    sprites.creation[4] = sprite_grab(388, 34, 30, 30);
    sprites.creation[5] = sprite_grab(418, 34, 30, 30);
}

void closeSprites(){
    for(int i = 0; i < 4; i++){
        al_destroy_bitmap(sprites.tank[i]);
        al_destroy_bitmap(sprites.enemies[i]);
        al_destroy_bitmap(sprites.shots[i]);
    }

    al_destroy_bitmap(sprites.stone);
    al_destroy_bitmap(sprites.bush);
    al_destroy_bitmap(sprites.block[0]);
    al_destroy_bitmap(sprites.block[1]);

    for(int i = 0; i < 6; i++){
        if(i < 3)
            al_destroy_bitmap(sprites.explosion[i]);
        al_destroy_bitmap(sprites.creation[i]);
    }

    al_destroy_bitmap(sprites.eagle);
}

void closeSound(){
    al_destroy_sample(sounds.explosion);
    al_destroy_sample(sounds.creation);
    al_destroy_sample(sounds.shot);
}

void initSound(){
    sounds.explosion = al_load_sample("resources/explosion.wav");
    check(sounds.explosion, "som explosão");
    sounds.creation = al_load_sample("resources/creation.wav");
    check(sounds.explosion, "som criação");
    sounds.shot = al_load_sample("resources/shot.wav");
    check(sounds.explosion, "som tiro");
}

int initDisplay (){
    //inicia as configurações do display
    initConf();

    //inicia sons
    initSound();

    return 1;
}

void initMenuDisplay(){
    // pega logo
    menuDisplay.logo = sprite_grab(133, 275, 383, 143);

    menuDisplay.startGame = malloc(sizeof(char)*30);
    if(!menuDisplay.startGame){
        fprintf(stderr, "Não foi possível alocar espaço para o texto de inicio do jogo !\n");
        exit(1);
    }
    strcpy(menuDisplay.startGame, "Pressione 's' para começar");
    
    menuDisplay.help = malloc(sizeof(char)*30);
    if(!menuDisplay.help){
        fprintf(stderr, "Não foi possível alocar espaço para o texto de ajuda !\n");
        exit(1);
    }
    strcpy(menuDisplay.help, "h - Ajuda");
    
    menuDisplay.finish = malloc(sizeof(char)*30);
    if(!menuDisplay.help){
        fprintf(stderr, "Não foi possível alocar espaço para o texto de sair !\n");
        exit(1);
    }
    strcpy(menuDisplay.finish, "Esc - Sair");
}

void closeMenu(){
    al_destroy_bitmap(menuDisplay.logo);
    free(menuDisplay.startGame);
    free(menuDisplay.finish);
    free(menuDisplay.help);
}


void startFPS(){
    //inicia a contagem do timer
    al_start_timer(timer);
}

void drawDisplay (GameObject_t *obj){
    int type = obj->type;
    int direction = obj->direction;
    int x = obj->x, y = obj->y;
    //desenha o sprite "sprite" na tela na posicação (x, y)
    ALLEGRO_BITMAP* sprite;
    if(type == TANK){
        sprite = sprites.tank[direction];
    }
    if(type == ENEMIES){
        sprite = sprites.enemies[direction];
    }
    if(type == SHOT || type == ENEMIE_SHOT){
        sprite = sprites.shots[direction];
    }
    if(type == BLOCK){
        sprite = sprites.block[direction];
    }

    al_draw_bitmap(sprite, x, y, 0);
}

void drawWall(GameObject_t *obj, int typeWall){
    int direction = obj->direction;
    int x = obj->x, y = obj->y;
    //desenha o sprite "sprite" na tela na posicação (x, y)
    ALLEGRO_BITMAP* sprite;
    switch (typeWall){
        case SPRITE_BRICK:
            sprite = sprites.block[direction];
            break;
        case SPRITE_STONE:
            sprite = sprites.stone;
            break;
        case SPRITE_BUSH:
            sprite = sprites.bush;
            break;
    default:
        break;
    }
    al_draw_bitmap(sprite, x, y, 0);
}

void drawInfo(){
    //desenha margem
    al_draw_filled_rectangle(BATTLE_FIELD_W, 0, DISPLAY_WIDHT, DISPLAY_HEIGH, al_map_rgb(153, 153, 153));
    //al_draw_filled_rectangle(BATTLE_FIELD_W, 0, DISPLAY_WIDHT, DISPLAY_HEIGH, al_map_rgb(38, 38, 38));
}

void drawMenu(){
    al_draw_bitmap(menuDisplay.logo, 85, 70, 0);
    
    int margin = 20;
    int x = (DISPLAY_WIDHT/2) - (al_get_text_width(font, menuDisplay.startGame) / 2);
    al_draw_text(font, al_map_rgb(255, 255, 255), x, 233, 0, menuDisplay.startGame);

    al_draw_text(font, al_map_rgb(255, 255, 255), margin, DISPLAY_HEIGH - margin, 0, menuDisplay.help);
    
    x = DISPLAY_WIDHT - al_get_text_width(font, menuDisplay.finish) - margin;
    al_draw_text(font, al_map_rgb(255, 255, 255), x, DISPLAY_HEIGH - margin, 0, menuDisplay.finish);
}

void drawHelp(){
    /*
    Instruções:
    Utilize as setas para se mover a tecla z para atirar
    A cada 3 seg. o valor em pontos para cada tank destruido diminui
    Não permita que os inimigos destruam a àguia

    Autor:
    Gustavo H. da Silva Barbosa
    UFPR
    github.com/GustavoHen12

    Cheat code: ghost
    */

    char instructions [] = "Instruções:\nUtilize as setas para se mover, e a tecla z para atirar\nA cada 3 seg. o valor em pontos para cada tank destruido diminui\nNão permita que os inimigos destruam a àguia";     
    char author[] = "Autor:\nGustavo H. da Silva Barbosa\nUFPR\ngithub.com/GustavoHen12";
    char cheatCode[] = "Cheat code: ghost";
    char exit[] = "m - Voltar ao menu";

    int margin = 20;
    int sizeLine = al_get_font_line_height(font);
    al_draw_multiline_text(font, al_map_rgb(255, 255, 255), margin, margin, DISPLAY_HEIGH - margin, sizeLine, ALLEGRO_ALIGN_LEFT, instructions);

    al_draw_multiline_text(font, al_map_rgb(255, 255, 255), margin, (sizeLine*12) + margin, DISPLAY_HEIGH - margin, sizeLine, ALLEGRO_ALIGN_LEFT, author);
    al_draw_text(font, al_map_rgb(255, 255, 255), margin, (sizeLine*20) + margin, 0, cheatCode);
    
    al_draw_text(font, al_map_rgb(255, 255, 255), margin, DISPLAY_HEIGH - margin, 0, exit);
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
    al_destroy_bitmap(_sheet);
}

void playSound(int type){
    if(type == FX_TYPE_CREATION)
        al_play_sample(sounds.creation, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    if(type == FX_TYPE_EXPLOSION)
        al_play_sample(sounds.explosion, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    if(type == FX_TYPE_SHOT)
        al_play_sample(sounds.shot, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

// ------------ Efeitos -----------

void fx_init()
{
    for(int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(int x, int y, int type)
{
    for(int i = 0; i < FX_N; i++)
    {
        if(fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].used = true;
        fx[i].type = type;
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
        if(fx[i].type == FX_TYPE_CREATION){
            if(fx[i].frame == (CREATION_FRAMES * 8))
                fx[i].used = false;
        }else{
            if(fx[i].frame == (EXPLOSION_FRAMES * 8))
                fx[i].used = false;
        }
    }
}

int fx_finished(int type){
    int finish = 0;
    int frames = type == FX_TYPE_CREATION ? CREATION_FRAMES : EXPLOSION_FRAMES;
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        if(fx[i].type == type){
            if((fx[i].frame + 1) == (frames * 8))
                finish = 1;
        }
    }

    return finish;
}

void fx_draw()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 8;
        ALLEGRO_BITMAP *bmp = fx[i].type == FX_TYPE_CREATION ? 
                                            sprites.creation[frame_display]
                                            :sprites.explosion[frame_display];

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}

