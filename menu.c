#include <stdio.h>
#include <stdlib.h>/*
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>*/
#include "menu.h"

//==================================================== DRAW_MENU ========================================================//
void Menu_Draw(struct Game *game) {
    al_draw_bitmap(game->menu.menu_bitmap,0,0,0);
    al_set_target_bitmap(al_get_backbuffer(game->display));
    al_draw_bitmap_region(game->menu.mills, 0,game->menu.mills_y,   82,85,   799,485,  0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,0,  231,64,  game->menu.cldpos1*1.2+300,0, 231,64, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos1+53,95, 213,44, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,108,  245,54,  game->menu.cldpos1*1.5+723,24, 245,54, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,162,  190,41,  game->menu.cldpos1*0.8+665,138, 190,41, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,0,  231,64,  game->menu.cldpos2+475,175, 231*0.7,64*0.7, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos2*1.15+250,70, 213*0.9,44*0.9, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,108,  245,54,  game->menu.cldpos2*1.1+22,220, 245*0.8,54*0.8, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,162,  190,41,  game->menu.cldpos3+885,275, 190*0.7,41*0.7, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos3*0.7+520,290, 213*0.4,44*0.4, 0);
    al_draw_bitmap(game->menu.smok,0,95,0);
//PRZYCISKI//
    if(game->gamestate==0){
        al_draw_bitmap_region(game->menu.buttons, 0,0,   195,43,   635,41,  0); //new game
        if(game->play.dragon_lives>0)
            al_draw_bitmap_region(game->menu.buttons, 0,43,  195,43,   635,119, 0); //resume game
        else
            al_draw_bitmap_region(game->menu.buttons, 0,215,  195,43,   635,119, 0); //resume game przygaszony
        al_draw_bitmap_region(game->menu.buttons, 0,86,  195,43,   635,197, 0); //options
        al_draw_bitmap_region(game->menu.buttons, 0,129, 195,43,   635,275, 0); //quit
    }
//WYNIKI I INSTRUKCJA GRY//
    if(game->gamestate==2){
        al_draw_bitmap_region(game->menu.buttons, 195,0, 132,211,   698,41, 0); //tablica wyników
        al_draw_bitmap_region(game->menu.buttons, 0,258, 498,211,   118,41, 0); //tablica informacji o grze
        al_draw_bitmap_region(game->menu.buttons, 0,172, 195,43,    635,275, 0); //przycisk 'menu'

        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,84,  ALLEGRO_ALIGN_LEFT, "1.     %i", game->table[0]); // stara poprawiona wysokość = 77 i co 20
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,104, ALLEGRO_ALIGN_LEFT, "2.     %i", game->table[1]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,124, ALLEGRO_ALIGN_LEFT, "3.     %i", game->table[2]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,144, ALLEGRO_ALIGN_LEFT, "4.     %i", game->table[3]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,164, ALLEGRO_ALIGN_LEFT, "5.     %i", game->table[4]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,184, ALLEGRO_ALIGN_LEFT, "6.     %i", game->table[5]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,204, ALLEGRO_ALIGN_LEFT, "7.     %i", game->table[6]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 737,224, ALLEGRO_ALIGN_LEFT, "8.     %i", game->table[7]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,61,  ALLEGRO_ALIGN_LEFT, "Press         or          to move up and         or          to move down.");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,81,  ALLEGRO_ALIGN_LEFT, "Press                    or right click mouse to fire a fiery bullet!");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,101, ALLEGRO_ALIGN_LEFT, "To pause game or enter menu press          .");
        //al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,121, ALLEGRO_ALIGN_LEFT, "");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,141, ALLEGRO_ALIGN_LEFT, "You've got three lives.");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,161, ALLEGRO_ALIGN_LEFT, "Avoid thrown pitchforks - they will kill you!");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,181, ALLEGRO_ALIGN_LEFT, "Flying into a cloud of flour disables you for a while.");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,201, ALLEGRO_ALIGN_LEFT, "Do not fly too low - you will fall and die.");
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 138,221, ALLEGRO_ALIGN_LEFT, "Shepherd boys fire exploding sheep defragmentators!");
    }
//BITMAPA KURSORA//
    al_draw_bitmap(game->menu.cursor,game->pos_x, game->pos_y, 0);
}
//==================================================== UPDATE_FILE ========================================================//
void Update_File(struct Game *game) {
    int i,tmp,x=game->play.score;
    FILE *config_file;
    config_file=fopen("game.ini","r");
    for(i=0; i<8; i++)
        fscanf(config_file,"%d", &(game->table[i]));
    fclose(config_file);

    for(i=0; i<8; i++){
        if(x == game->table[i]) break;
        else if(x > game->table[i]){
            tmp=game->table[i];
            game->table[i]= x;
            x=tmp;
        }
    }
    config_file=fopen("game.ini","w");
    for(i=0; i<8; i++)
        fprintf(config_file,"%d\n",game->table[i]);
    fclose(config_file);
}
