#ifndef MAIN_H
#define MAIN_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

struct Obstacles {
    int state; //0-zużyte, 1-mąka, 2-widły, 3-owczy defragmentator
    float x1;
    float y1;
    float a;
    float b;
    float x3;
    float y3;
    float frame_y;
    struct Obstacles *next;
};

struct Fodder {
    int type; //0-uzbrojony, 1-nieuzbrojony, 2-owca, 3-wiatrak, 4-domek
    int lives; //1 dla wieśniakó i owiec, 3 dla domków
    float x1;
    float y1;
    float x2;
    float y2;
    struct Fodder *next;
};

struct Fire {
    int exist; //1-leci, 2-wybuch, 3-po wybuchu lub wyleciał za ekran
    float fire_pos_x;
    float fire_pos_y;
    float fire_pos_x2;
    float fire_pos_y2;
    float a;
    float b;
    float x;
    float y;
    struct Fire *next;
};

struct Smoke {
    int x;
    int y;
    int frame_x;
    struct Smoke *next;
};

struct Play {
    ALLEGRO_BITMAP *background, *grass1, *grass2, *mtn1, *mtn2, *trees1, *trees2, *dragon, *score_table, *fire, *fodder, *pitchfork;
    ALLEGRO_FONT *total_score;
    float for_mtn1, for_mtn2, for_grs1, for_grs2, dragon_frame_x, dragon_pos_y, dragon_pos_x, fire_pos_x, fire_pos_y, fodder_x, fodder_mill_y, flour_y;//, minus;
    int i, i2,  i3, score, score_villagers, score_sheeps, score_huts, dragon_lives;
    bool down, info, dragon_floured;
};

struct Menu {
    ALLEGRO_BITMAP  *menu_bitmap, *mills, *cloud, *smok, *buttons, *cursor;
    ALLEGRO_SAMPLE *song;
    float cldpos1, cldpos2, cldpos3, mills_y;
};

struct Game {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_FONT *sfont;
    int WIDTH, HEIGHT, pos_x, pos_y, gamestate, fireonce;
    int table[10];
    struct ALLEGRO_KEYBOARD_STATE keyboard;
    struct Menu menu;
    struct Play play;
    struct Fire *first, *prev, *last;
    struct Fodder *ffirst, *fprev, *flast;
    struct Smoke *sfirst, *sprev;
    struct Obstacles *ofirst, *oprev;
};

#endif // MAIN_H_INCLUDED
