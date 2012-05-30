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
            al_draw_bitmap_region(game->menu.buttons, 0,413,  195,43,   635,119, 0); //resume game przygaszony
        al_draw_bitmap_region(game->menu.buttons, 0,86,  195,43,   635,197, 0); //scores
        al_draw_bitmap_region(game->menu.buttons, 0,129, 195,43,   635,275, 0); //quit
    }
//WYNIKI//
    if(game->gamestate==2){
        al_draw_bitmap_region(game->menu.buttons, 0,172, 195,198,   635,41, 0); //tablica wyników
        al_draw_bitmap_region(game->menu.buttons, 0,250, 195,120,   635,132, 0); //tablica wyników - przedłużenie
        al_draw_bitmap_region(game->menu.buttons, 0,370, 195,43,   635,275, 0); //przycisk 'menu'
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,50, ALLEGRO_ALIGN_LEFT, "1.     %i", game->table[0]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,70, ALLEGRO_ALIGN_LEFT, "2.     %i", game->table[1]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,90, ALLEGRO_ALIGN_LEFT, "3.     %i", game->table[2]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,110, ALLEGRO_ALIGN_LEFT, "4.     %i", game->table[3]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,130, ALLEGRO_ALIGN_LEFT, "5.     %i", game->table[4]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,150, ALLEGRO_ALIGN_LEFT, "6.     %i", game->table[5]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,170, ALLEGRO_ALIGN_LEFT, "7.     %i", game->table[6]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,190, ALLEGRO_ALIGN_LEFT, "8.     %i", game->table[7]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,210, ALLEGRO_ALIGN_LEFT, "9.     %i", game->table[8]);
        al_draw_textf(game->sfont, al_map_rgb(70,64,58), 710,230, ALLEGRO_ALIGN_LEFT, "10.   %i", game->table[9]);
    }
//BITMAPA KURSORA//
    al_draw_bitmap(game->menu.cursor,game->pos_x, game->pos_y, 0);

	game->menu.cldpos1-=(0.4); //zmiana pozycji chmur//
	if (game->menu.cldpos1<-1090)  game->menu.cldpos1=948;
	game->menu.cldpos2-=(0.4); //zmiana pozycji chmur//
	if (game->menu.cldpos2<-700)  game->menu.cldpos2=948;
	game->menu.cldpos3-=(0.4); //zmiana pozycji chmur//
	if (game->menu.cldpos3<-1000)  game->menu.cldpos3=948;
    if(game->play.i2==5){   //klatki animacji wiatraków
        game->menu.mills_y+=85;
        if (game->menu.mills_y>=680)  game->menu.mills_y=0;
        game->play.i2=0;
    }
    game->play.i2++;
}
//==================================================== UPDATE_FILE ========================================================//
void Update_File(struct Game *game) {
    int i,tmp,x=game->play.score;
    FILE *config_file;
    config_file=fopen("game.ini","r");
    for(i=0; i<10; i++)
        fscanf(config_file,"%d", &(game->table[i]));
    fclose(config_file);

    for(i=0; i<10; i++){
        if(x == game->table[i]) break;
        else if(x > game->table[i]){
            tmp=game->table[i];
            game->table[i]= x;
            x=tmp;
        }
    }
    config_file=fopen("game.ini","w");
    for(i=0; i<10; i++)
        fprintf(config_file,"%d\n",game->table[i]);
    fclose(config_file);
}
