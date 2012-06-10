#ifndef MAIN_H
#define MAIN_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

struct Obstacles {
    int state; //0-zużyte, 1-mąka, 2-widły, 3-owczy defragmentator, 4-owcze fragmenty
    float x1;
    float y1;
    float a;
    float b; //p
    float q; //q
    float x3;
    float y3;
    float frame_y;
    struct Obstacles *next;
};

struct Fodder {
    int type; //0-uzbrojony, 1-nieuzbrojony, 2-owca, 3-wiatrak, 4-domek
    int lives; //1 dla wieśniakó i owiec, 3 dla domków
    bool launch; //czy pastuch strzelił owcą?
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
    float N;
    struct Fire *next;
};

struct Smoke {
    int type; //1-dym, 2-krew
    float x;
    float y;
    int frame_x;
    struct Smoke *next;
};

struct Play {
    ALLEGRO_BITMAP *background, *grass1, *grass2, *mtn1, *mtn2, *trees1, *trees2, *dragon, *score_table, *fire, *fodder, *pitchfork, *sheep_bmp;
    ALLEGRO_FONT *total_score;
    ALLEGRO_SAMPLE *psong, /* *expl*/ *flap1, *flap2, *flap3, *flap4, *die, *hit1, *hit2, /* *hutexpl*/ *whoosh1, *whoosh2, *lfire, *sheep, *hiss;
    ALLEGRO_SAMPLE_INSTANCE *in_psong, /* *in_expl*/ *in_flap1, *in_flap2, *in_flap3, *in_flap4, *in_die, *in_hit1, *in_hit2, /* *in_hutexpl*/ *in_whoosh1, *in_whoosh2, *in_lfire, *in_sheep, *in_hiss;
    float for_mtn1, for_mtn2, for_grs1, for_grs2, dragon_frame_x, dragon_pos_y, dragon_pos_x, fire_pos_x, fire_pos_y, fodder_x, fodder_mill_y, flour_y;
    int i, i2,  i3, score, score_villagers, score_sheeps, score_huts, dragon_lives;
    bool down, info, dragon_floured;
};

struct Menu {
    ALLEGRO_BITMAP  *menu_bitmap, *mills, *cloud, *smok, *buttons, *cursor;
    ALLEGRO_SAMPLE *song, *click1, *click2;
    ALLEGRO_SAMPLE_INSTANCE *in_song, *in_click1, *in_click2;
    float cldpos1, cldpos2, cldpos3, mills_y;
};

struct Game {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_FONT *sfont;
    ALLEGRO_MIXER *mixer, *mixer_music, *mixer_sfx, *mixer_sfx2;
    ALLEGRO_VOICE *voice;
    int WIDTH, HEIGHT, pos_x, pos_y, fireonce;
    int gamestate; //0 = menu; 1 = gra; 2 = wyniki
    int table[8];  //tablica wyników
    float sfxvol, musvol;
    struct ALLEGRO_KEYBOARD_STATE keyboard;
    struct Menu menu;
    struct Play play;
    struct Fire *first, *prev;
    struct Fodder *ffirst, *fprev;
    struct Smoke *sfirst, *sprev;
    struct Obstacles *ofirst, *oprev;
};

#endif // MAIN_H_INCLUDED
