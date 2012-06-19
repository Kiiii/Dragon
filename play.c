/*#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>*/
#include <stdio.h>
#include <math.h>
#include "play.h"
#include "lists.h"

//==================================================== PLAY_GAME ========================================================//
void Play_Game(struct Game *game)   {
/*ANIMACJA T£A*/
    al_draw_bitmap(game->play.background, 0,0, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,0,  231,64,  game->menu.cldpos1*1.2+300,0, 231,64, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos1+53,95, 213,44, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,108,  245,54,  game->menu.cldpos1*1.5+723,24, 245,54, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,162,  190,41,  game->menu.cldpos1*0.8+665,138, 190,41, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,0,  231,64,  game->menu.cldpos2+475,175, 231*0.7,64*0.7, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos2*1.15+250,70, 213*0.9,44*0.9, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,108,  245,54,  game->menu.cldpos2*1.1+22,220, 245*0.8,54*0.8, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,162,  190,41,  game->menu.cldpos3+885,275, 190*0.7,41*0.7, 0);
    al_draw_scaled_bitmap(game->menu.cloud,  0,64,  213,44,  game->menu.cldpos3*0.7+520,290, 213*0.4,44*0.4, 0);
    al_draw_bitmap(game->play.mtn1, game->play.for_mtn1,340, 0); //I połówka trawy
    al_draw_bitmap(game->play.mtn2, game->play.for_mtn2,340, 0); //II połówka trawy
    al_draw_bitmap(game->play.grass1, game->play.for_grs1,492, 0); //I połówka trawy
    al_draw_bitmap(game->play.grass2, game->play.for_grs2,492, 0); //II połówka trawy
    al_draw_bitmap(game->play.trees1, game->play.for_grs1,438, 0); //las
    al_draw_bitmap(game->play.trees2, game->play.for_grs2,438, 0); //las
/*WIEŚNIAKI*/
    if(game->ffirst){
        struct Fodder *ftmp;
        ftmp=game->ffirst;
        while(ftmp){
            if(ftmp->type==0) {  //uzbrojony
                al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x, 0,   25,22,   ftmp->x1,ftmp->y1,  0);
                if((rand() % 100 < 2) && ftmp->x1>game->play.dragon_pos_x+450 && game->play.dragon_lives>0){
                    Add_Obstacles(game,ftmp->x1,ftmp->y1,2,game->play.dragon_pos_x+15,game->play.dragon_pos_y+110);  //rzut widłami
                    if(rand() % 100 < 30){  //dźwięk rzutu wideł
                        if(rand() % 2 == 1){  game->play.in_whoosh1 = al_create_sample_instance(game->play.whoosh1);  al_attach_sample_instance_to_mixer(game->play.in_whoosh1, game->mixer_sfx);  al_set_sample_instance_gain(game->play.in_whoosh1, game->sfxvol-0.11);   al_play_sample_instance(game->play.in_whoosh1);   }
                        else {  game->play.in_whoosh2 = al_create_sample_instance(game->play.whoosh2);  al_attach_sample_instance_to_mixer(game->play.in_whoosh2, game->mixer_sfx);  al_set_sample_instance_gain(game->play.in_whoosh2, game->sfxvol-0.18);   al_play_sample_instance(game->play.in_whoosh2);   }
                    }
                    else al_play_sample_instance(game->play.in_whoosh2);
                }
            }
            if(ftmp->type==1) {  //pastuch owcowy
                al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+25, 0,   25,22,   ftmp->x1,ftmp->y1,  0);
                if((rand() % 100  <25) && ftmp->x1>600 && game->play.dragon_lives>0 && ftmp->launch==false){
                    Add_Obstacles(game, ftmp->x1, ftmp->y1, 3, 0, 0); //wystrzelenie owczego defragmentatora
                    ftmp->launch=true;
                }
            }
            if(ftmp->type==2) {  //owca
                al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+50, 0,   20,15,   ftmp->x1,ftmp->y1,  0);
            }
            if(ftmp->type==4) {  //domek
                if(ftmp->lives==3) //animacje w zależności od ilości żyć - rozwalanie budowli
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+125, 0,   62,54,   ftmp->x1,ftmp->y1,  0);
                else if(ftmp->lives==2)
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+125, 54,   62,54,   ftmp->x1,ftmp->y1,  0);
                else if(ftmp->lives==1)
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+125, 108,   62,54,   ftmp->x1,ftmp->y1,  0);
            }
            if(ftmp->type==3) {   //wiatrak
                if(ftmp->lives==3)  //animacje w zależności od ilości żyć - rozwalanie budowli
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+70, game->play.fodder_mill_y,   55,60,   ftmp->x1,ftmp->y1,  0);
                else if(ftmp->lives==2)
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+70, 480,   55,60,   ftmp->x1,ftmp->y1,  0);
                else if(ftmp->lives==1)
                    al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+70, 540,   55,60,   ftmp->x1,ftmp->y1,  0);
                if(rand() % 100  < 1)
                    Add_Obstacles(game,ftmp->x1,ftmp->y1,1,0,0); //wypuszczenie mąki
            }
            if(game->play.dragon_lives>0){ //przesuwa foddery, o ile smok żyje
                ftmp->x1-=5;
                ftmp->x2-=5;
            }
            ftmp=ftmp->next;
        }
    }
/*POCISK*/
    if(game->first){
        struct Fire *tmp;
        tmp=game->first;
            while(tmp!=NULL){
                   // al_draw_filled_circle(tmp->fire_pos_x2, tmp->fire_pos_y2,1,al_map_rgb(0,0,255)); //niebieskie - celownik
                if((tmp->x > al_get_display_width(game->display)) || (tmp->y < 0) || (tmp->x < 0) || (tmp->y > al_get_display_height(game->display))) //pocisk wyleciał za ekran - przeznaczony na straty
                    tmp->exist=3;
                else if(tmp->exist==2){
                    al_draw_bitmap_region(game->play.fodder, 187,0, 38,29, tmp->x,tmp->y,  0); //wybuch pocisku
                    tmp->exist=3;
                }
                else if(tmp->exist==1) {
                    tmp->y = tmp->a * tmp->x + tmp->b;
                    al_draw_bitmap_region(game->play.fire, 0,0, 15,15, tmp->x,tmp->y,  0); //ognisty pocisk
                    if(rand() % 100  <13)
                        Add_Smoke(game, tmp->x, tmp->y, 1); //puszczanie dymu
                }
                tmp->x+=(tmp->N * 6);
                tmp=tmp->next;
            }
    }
/*DYM OD POCISKÓW*/
    if(game->sfirst){
        struct Smoke *stmp;
        stmp=game->sfirst;
            while(stmp){
                if(stmp->type==1){
                    al_draw_bitmap_region(game->play.fire,    stmp->frame_x, 0,   15,15,   stmp->x,stmp->y,  0);
                    if((rand() % 100 <50) && (stmp->frame_x <= 180))
                        stmp->frame_x+=15;
                }
                else {
                    al_draw_bitmap_region(game->play.fire,    stmp->frame_x, 15,   15,15,   stmp->x,stmp->y,  0);
                    if((rand() % 100 <50) && (stmp->frame_x <= 180))
                        stmp->frame_x+=30;
                }
                stmp=stmp->next;
            }
    }
/*ANIMACJA SMOKA*/
    if(game->play.down==false && game->play.dragon_lives>0) al_draw_bitmap_region(game->play.dragon,   game->play.dragon_frame_x, 0,   152, 199,   0,game->play.dragon_pos_y,  0);
    else if(game->play.down==true && game->play.dragon_lives>0) al_draw_bitmap_region(game->play.dragon,   152, 0,   152,199,   0,game->play.dragon_pos_y,  0);
    if(game->play.dragon_floured==true){   //wyświetlanie chmur mąki, gdy smok wleci w takową
        al_draw_tinted_bitmap_region(game->play.fodder, al_map_rgba_f(0.5,0.5,0.5,0.5),  245,game->play.flour_y,   48,38,   game->play.dragon_pos_x-20,game->play.dragon_pos_y+80, 0);
        al_draw_tinted_bitmap_region(game->play.fodder, al_map_rgba_f(0.5,0.5,0.5,0.5),  245,game->play.flour_y,   48,38,   game->play.dragon_pos_x-40,game->play.dragon_pos_y+78, 0);
        al_draw_tinted_bitmap_region(game->play.fodder, al_map_rgba_f(0.5,0.5,0.5,0.5),  245,game->play.flour_y,   48,38,   game->play.dragon_pos_x-60,game->play.dragon_pos_y+80, 0);
    }
/*PRZESZKODY LATAJĄCE*/
    if(game->ofirst){
        struct Obstacles *otmp;
        otmp=game->ofirst;
        while(otmp){
           // printf("%d\n",otmp->state);
            if(otmp->state==1){ //mąka
                al_draw_bitmap_region(game->play.fodder,   245,otmp->frame_y,   48,38,   otmp->x1,otmp->y1,  0);
                if((rand() % 100) <25)   otmp->frame_y+=38;
                if(otmp->frame_y>=494)   otmp->frame_y=0;
                if(game->play.dragon_lives>0)
                    otmp->x1-=((2+rand() % 3));
                otmp->y1-=2;
                if(otmp->y1<-20 || otmp->x1<-20) otmp->state=0; //mąka wyleciała za ekran
            }
            else if(otmp->state==2){ //widły
                al_draw_rotated_bitmap(game->play.pitchfork, 10,10, otmp->x3,otmp->y3,  otmp->a*otmp->a*(54.235),   0); //obrót o 25 całkiem ładny
                otmp->x3-=8;
                otmp->y3 = otmp->a * otmp->x3 + otmp->b;
                if(otmp->y3<-20 || otmp->x3<-20) otmp->state=0;
             }
            else if(otmp->state==3){ //owca
                al_draw_rotated_bitmap(game->play.sheep_bmp, 18,15, otmp->x3,otmp->y3,  otmp->a*(1054.235)+ 2,   0);
                if(otmp->y3 <= -0.4*otmp->q){ //wybuch
                    al_draw_bitmap_region(game->play.fodder, 187,0, 38,29, otmp->x3,otmp->y3,  0);
                    Add_Obstacles(game, otmp->x3, otmp->y3, 4, 0, (rand() % 170)); //defragment 1!
                    Add_Obstacles(game, otmp->x3, otmp->y3, 4, 0, (170 + rand() % 170)); //defragment 2!
                    Add_Obstacles(game, otmp->x3, otmp->y3, 4, 0, (340 + rand() % 170)); //defragment 3!
                    if(rand() % 2 == 1)   Add_Obstacles(game, otmp->x3, otmp->y3, 4, 0, (rand() % 500)); //defragment 4!
                    Add_Obstacles(game, otmp->x3, otmp->y3, 5, 0, (900 - rand() % 200)); //defragment 5! w prawo
                    Add_Obstacles(game, otmp->x3, otmp->y3, 5, 0, (rand() % 300)); //defragment 6! w prawo
                    otmp->state=0;
                    game->play.in_sheep = al_create_sample_instance(game->play.sheep);   al_attach_sample_instance_to_mixer(game->play.in_sheep, game->mixer_sfx);   al_set_sample_instance_gain(game->play.in_sheep, game->sfxvol-0.15);   al_play_sample_instance(game->play.in_sheep);
                }
                otmp->x3-=5.5;
                otmp->y3=((otmp->a)*pow((otmp->x3-otmp->b),2)+(otmp->q))+510;
                if(otmp->y3>600 || otmp->x3<-20) otmp->state=0;
            }
            else if(otmp->state==4 || otmp->state==5){ //owczy defragmentator
                al_draw_bitmap_region(game->play.fire,    otmp->frame_y, 15,   15,15,   otmp->x3,otmp->y3,  0);
                if(otmp->state==4) //w lewo leci
                    otmp->x3-=8;
                else
                    otmp->x3+=5; //w prawo leci
                otmp->y3 = otmp->a * otmp->x3 + otmp->b;
                if(rand() % 100  <40)   Add_Smoke(game, otmp->x3, otmp->y3, 2); //krew od defragmentów
                if(otmp->y3<-20 || otmp->y3>600 || otmp->x3<-20 || otmp->x3>950) otmp->state=0;
            }
            otmp=otmp->next;
        }
    }
/*TABLICA WYNIKÓW*/
    al_draw_bitmap(game->play.score_table,332,6,0); //bitmapa tablicy wyników
    if(game->play.dragon_lives>=1){
        al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+225,0,   18,17,   447,75,  0); //bitmapa serduszka
        if(game->play.dragon_lives>=2){
            al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+225,0,   18,17,   465,75,  0); //bitmapa serduszka
            if(game->play.dragon_lives==3)
                al_draw_bitmap_region(game->play.fodder,   game->play.fodder_x+225,0,   18,17,   483,75,  0); //bitmapa serduszka
        }
    }
/*WSZELAKIE NAPISY PODCZAS GRY*/
    al_draw_textf(game->sfont, al_map_rgb(201,185,160), al_get_display_width(game->display)*0.5, 51, ALLEGRO_ALIGN_CENTRE, "Score: %i", game->play.score);
    al_draw_textf(game->sfont, al_map_rgb(201,185,160), 380, 22, ALLEGRO_ALIGN_LEFT, "%i", game->play.score_villagers);
    al_draw_textf(game->sfont, al_map_rgb(201,185,160), 458, 22, ALLEGRO_ALIGN_LEFT, "%i", game->play.score_sheeps/2);
    al_draw_textf(game->sfont, al_map_rgb(201,185,160), 544, 22, ALLEGRO_ALIGN_LEFT, "%i", game->play.score_huts/3);
    if(game->play.info==true && game->play.dragon_lives==2)
        al_draw_textf(game->sfont, al_map_rgb(70,64,58),  al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.4, ALLEGRO_ALIGN_CENTRE, "%d lives left!",game->play.dragon_lives);
    else if(game->play.info==true && game->play.dragon_lives==1)
        al_draw_textf(game->sfont, al_map_rgb(70,64,58),  al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.4, ALLEGRO_ALIGN_CENTRE, "%d life left!",game->play.dragon_lives);
    else if(game->play.dragon_lives==0)
        al_draw_textf(game->sfont, al_map_rgb(70,64,58),  al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.4, ALLEGRO_ALIGN_CENTRE, "GAME OVER! Press 'Q' to enter menu.");
/*BITMAPA KURSORA*/
    al_draw_bitmap(game->menu.cursor,game->pos_x, game->pos_y, 0);
}
//==================================================== INCREMENT_ALL ========================================================//
void Increment_All(struct Game *game){
 if(game->gamestate==1){
    game->play.i2+=1;   //inkrementacja zmiennej od szybkości zmiany klatek animacji wiatraków
    if(game->play.i2==6) {
        game->play.fodder_mill_y+=60;
        if(game->play.fodder_mill_y>=480) game->play.fodder_mill_y=0;

        if(game->play.dragon_lives>0){  game->play.flour_y+=38; //jeśli smok żyje, to zmienną od zmączenia inkrementuje
            if(game->play.flour_y>=494) game->play.flour_y=0;
        }
        game->play.i2=0;
    }
    if(game->play.dragon_lives>0){          //tło się przesuwa tylko wtedy, gdy smok żyje
        if (game->play.for_mtn1 <= -948) game->play.for_mtn1=948;
        game->play.for_mtn1-=3; //góry
        if (game->play.for_mtn2 <= -948) game->play.for_mtn2=948;
        game->play.for_mtn2-=3;
        game->play.for_grs1-=5; //trawa
        if (game->play.for_grs1 <= -950) game->play.for_grs1=950;
        game->play.for_grs2-=5;
        if (game->play.for_grs2 <= -950) game->play.for_grs2=950;
        game->menu.cldpos1-=(2); //chmury
        if (game->menu.cldpos1<-1090)  game->menu.cldpos1=948;
        game->menu.cldpos2-=(2);
        if (game->menu.cldpos2<-700)  game->menu.cldpos2=948;
        game->menu.cldpos3-=(2);
        if (game->menu.cldpos3<-1000)  game->menu.cldpos3=948;

        game->play.i+=1;    //inkrementacja zmiennej od szybkości zmiany klatek animacji smoka
        if (game->play.i == 8) {     //szybkość machnięć skrzydeł
            game->play.dragon_frame_x +=152;
            if(game->play.dragon_frame_x==456 && game->play.down==false){
                if(rand() % 4 == 0)
                    al_play_sample_instance(game->play.in_flap1);
                else if(rand() % 4 == 1)
                    al_play_sample_instance(game->play.in_flap2);
                else if(rand() % 4 == 2)
                    al_play_sample_instance(game->play.in_flap3);
                else
                    al_play_sample_instance(game->play.in_flap3);
            }
            if (game->play.dragon_frame_x>=1368){ //pozycja x w klatkach animacji smoka
                game->play.dragon_frame_x=0;
                if(game->play.info==true) game->play.info=false; //przerwanie wyświetlania infa o życiach
            }
            game->play.i=0;
        }
    }
    else { //wolniejsza zmiana pozycji chmur, gdy się temu smoku padnie
        game->menu.cldpos1-=(0.4);
        if (game->menu.cldpos1<-1090)  game->menu.cldpos1=948;
        game->menu.cldpos2-=(0.4);
        if (game->menu.cldpos2<-700)  game->menu.cldpos2=948;
        game->menu.cldpos3-=(0.4);
        if (game->menu.cldpos3<-1000)  game->menu.cldpos3=948;
    }
    if(game->play.dragon_floured==true){ //licznik zmiennej trwania zmączenia smoka
        game->play.dragon_pos_y+=1;
        game->play.i3+=1;
        if(game->play.i3>=150 && game->play.dragon_floured==true) game->play.dragon_floured=false; //przerwanie stanu zamączenia smoka
    }
    if(game->play.dragon_pos_y>(al_get_display_height(game->display)-200/*250*/) && game->play.dragon_lives>0) //strata życia po wleceniu w drzewka
        Revive_Dragon(game);
 }
 else if(game->gamestate!=1){   //nie jest stan aktulanej grywalności
    	game->menu.cldpos1-=(0.4);
        if (game->menu.cldpos1<-1090)  game->menu.cldpos1=948;
        game->menu.cldpos2-=(0.4);
        if (game->menu.cldpos2<-700)  game->menu.cldpos2=948;
        game->menu.cldpos3-=(0.4);
        if (game->menu.cldpos3<-1000)  game->menu.cldpos3=948;
        if(game->play.i2==5){       //klatki animacji wiatraków z menu
            game->menu.mills_y+=85;
            if (game->menu.mills_y>=680)  game->menu.mills_y=0;
                game->play.i2=0;
        }
    game->play.i2++;
 }
}
//==================================================== REVIVE_DRAGON ========================================================//
void Revive_Dragon(struct Game *game){
    game->play.info=true;
    game->play.dragon_lives-=1;
    game->play.i=0;
    if(game->play.dragon_lives!=0){
        Absolute_Free(game); //żeby foddery nie zniknęły, gdy smok padnie i zostanie się w stanie gry
        if(rand() % 2 == 0)
            al_play_sample_instance(game->play.in_hit1);
        else
            al_play_sample_instance(game->play.in_hit2);
    }
    else if(game->play.dragon_lives==0) al_play_sample_instance(game->play.in_die); //ryk całkowitego padnięcia smoka
    game->play.dragon_frame_x = 0;
    game->play.down = false;
    game->play.dragon_floured=false;
    game->play.dragon_pos_y = 100;
}
