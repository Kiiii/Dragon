#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lists.h"

//==================================================== FIRE_BULLET ========================================================//
void Fire_Bullet(struct Game *game)    {
//TWORZENIE POCISKÓW//
    struct Fire *tmp = (struct Fire *)malloc (sizeof(struct Fire));
    tmp->exist = 1;
    tmp->fire_pos_x = game->play.dragon_pos_x+5;
    tmp->fire_pos_y = game->play.dragon_pos_y+95;
    tmp->fire_pos_x2 = game->pos_x;
    tmp->fire_pos_y2 = game->pos_y;
    tmp->a = ((tmp->fire_pos_y - tmp->fire_pos_y2)/(tmp->fire_pos_x - tmp->fire_pos_x2));
    tmp->b = (tmp->fire_pos_y - tmp->a * tmp->fire_pos_x);
    tmp->x = tmp->fire_pos_x;
    tmp->y = (((tmp->a)*(tmp->x))+(tmp->b));
    tmp->N = ((tmp->fire_pos_x2 - tmp->fire_pos_x)/(sqrt(pow(tmp->fire_pos_x2 - tmp->fire_pos_x,2) + pow(tmp->fire_pos_y2 - tmp->fire_pos_y,2)))); //decyduje o przyroście x w funkcji
    tmp->next = NULL;
    if(game->first==NULL)
        game->first=tmp;
    else {
        tmp->next=game->first;
        game->first=tmp;
    }
}
//==================================================== FREE_FIRE ========================================================//
void Free_Fire(struct Game *game)   {
//USUWANIE POCISKÓW//
  if(game->first) {
    struct Fire *tmp;
    tmp=game->first;
    game->prev=NULL;
    while(tmp!=NULL){
        if(tmp->exist==3) {
            if(tmp==game->first) {
                game->first=game->first->next;
                free(tmp);
                tmp=game->first;
            }
            else {
                game->prev->next=tmp->next;
                free(tmp);
                tmp=game->prev->next;
            }
        }
        else {
            game->prev=tmp;
            tmp=tmp->next;
        }
    }
  }
}
//==================================================== CREATE_FODDER ========================================================//
void Create_Fodder(struct Game *game)   {
//TWORZENIE WIEŚNIAKÓW//
    struct Fodder *tmp = (struct Fodder *)malloc (sizeof(struct Fodder));
    tmp->type = ((rand() % 5 == 4)? (rand() % 3) : ((rand() % 5 == 4)? (rand() % 4): (rand() % 5))); //zmniejszenie prawdopodobieństwa wystąpienia domków
    tmp->lives=((tmp->type >2)? 3 : 1); //przydział punktów życia w zależnoœci od typu foddera
    tmp->launch=false;
    tmp->x1 = game->WIDTH-25;
    tmp->y2 = ((tmp->type >2)? (530 + (rand() % 65)) : (510 + rand() % 83));
    if(tmp->type<=1){       //wieśniaki
        tmp->x2 = tmp->x1+25;
        tmp->y1 = tmp->y2-25;   }
    else if(tmp->type==2) { //owca
        tmp->x2 = tmp->x1+24;
        tmp->y1 = tmp->y2-15;    }
    else if(tmp->type==4) { //domek
        tmp->x2 = tmp->x1+62;
        tmp->y1 = tmp->y2-52;    }
    else {                  //wiatrak
        tmp->x2 = tmp->x1+63;
        tmp->y1 = tmp->y2-60;    }
        tmp->next = NULL;

    if(game->ffirst==NULL)
        game->ffirst=tmp;
    else {
        tmp->next=game->ffirst;
        game->ffirst=tmp;
    }
}
//==================================================== FREE_FODDER ========================================================//
void Free_Fodder(struct Game *game)   {
//USUWANIE WIEŚNIAKÓW//
  if(game->ffirst) {
    struct Fodder *tmp;
    tmp=game->ffirst;
    game->fprev=NULL;
    while(tmp){
        if(tmp->x2<0 || tmp->lives<=0){
            if(tmp==game->ffirst) {
                game->ffirst=game->ffirst->next;
                free(tmp);
                tmp=game->ffirst;
            }
            else {
                game->fprev->next=tmp->next;
                free(tmp);
                tmp=game->fprev->next;
                }
        }
        else {
            game->fprev=tmp;
            tmp=tmp->next;
        }
    }
  }
}
//==================================================== BURN_FODDER ========================================================//
void Burn_Fodder(struct Game *game)   {
//SPRAWDZANIE KOLIZJI POCISKÓW Z WIEŚNIAKAMI//
    struct Fire *tmp;
    struct Fodder *ftmp;
    ftmp=game->ffirst;
    while(ftmp!=NULL){
        tmp=game->first;
        while(tmp!=NULL){
            if((tmp->exist==1) && (ftmp->lives>0) && (tmp->x >= (ftmp->x1-30)) && (tmp->x <= (ftmp->x2+30)) && (tmp->y >= ftmp->y1-30) && (tmp->y <= ftmp->y2+30) ){
                ftmp->lives-=1;
                if(ftmp->type==2 && ftmp->lives==0)
                    game->play.score_sheeps+=2;
                else if((ftmp->type==3 || ftmp->type==4) && ftmp->lives==0)
                    game->play.score_huts+=3;
                else if(ftmp->lives==0)
                    game->play.score_villagers++;
                tmp->exist=2;
            }
            tmp=tmp->next;
        }
        ftmp=ftmp->next;
    }
}
//==================================================== ADD_SMOKE ========================================================//
void Add_Smoke(struct Game *game, float x1, float y1, int type) {
//DODANIE DYMU//
    struct Smoke *tmp = (struct Smoke *)malloc (sizeof(struct Smoke));
    tmp->type=type;
    if(tmp->type==1)
        tmp->frame_x=15;
    else
        tmp->frame_x=45;
    tmp->x=x1;
    tmp->y=y1;
    tmp->next = NULL;
    if(game->sfirst==NULL)
        game->sfirst=tmp;
    else {
        tmp->next=game->sfirst;
        game->sfirst=tmp;
    }
}
//==================================================== FREE_SMOKE ========================================================//
void Free_Smoke(struct Game *game) {
//USUWANIE DYMU Z LISTY//
    struct Smoke *tmp;
    tmp=game->sfirst;
    game->sprev=NULL;
    while(tmp){
        if(tmp->frame_x > 180){
            if(tmp==game->sfirst) {
                game->sfirst=game->sfirst->next;
                free(tmp);
                tmp=game->sfirst;
            }
            else {
                game->sprev->next=tmp->next;
                free(tmp);
                tmp=game->sprev->next;
            }
        }
        else {
            game->sprev=tmp;
            tmp=tmp->next;
        }
    }
}
//==================================================== ADD_OBSTACLES ========================================================//
void Add_Obstacles(struct Game *game, float x1, float y1, int s, float x2, float y2){
//DODANIE PRZESZKÓD LATAJĄCYCH//
    struct Obstacles *tmp = (struct Obstacles *)malloc (sizeof(struct Obstacles));
    tmp->state=s;
    tmp->x1=x1;
    tmp->y1=y1;
    tmp->x3=tmp->x1;
    if(tmp->state==2 || tmp->state==4 || tmp->state==5){  //dla mąki i defragmentów owczych
        tmp->a=((y2 - tmp->y1)/(x2 - tmp->x1));
        tmp->b=(tmp->y1 - ((y2 - tmp->y1)/(x2 - tmp->x1))*tmp->x1);
        tmp->y3=tmp->a*tmp->x3+tmp->b;
    }
    else if(tmp->state==3){ //dla owiec
        tmp->b= tmp->x1-300; //p
        tmp->q= -(510 * 0.1 * (7 + rand() % 4));
        tmp->a= ((-tmp->q)/pow(tmp->x1 - tmp->b,2));
            tmp->y3=(tmp->a*pow(tmp->x3-tmp->b,2)+tmp->q)+510;  //y = a*(x-p)^2 + q
    }
    tmp->frame_y=0;
    if(tmp->state==4 || tmp->state==5){  //frame_x dla defragmentów
        if(rand() % 3 == 0)
            tmp->frame_y=0;
        else if(rand() % 3 == 1)
            tmp->frame_y=15;
        else
            tmp->frame_y=30;
    }
    tmp->next = NULL;
    if(game->ofirst==NULL)
        game->ofirst=tmp;
    else {
        tmp->next=game->ofirst;
        game->ofirst=tmp;
    }
}
//==================================================== FREE_OBSTACLES ========================================================//
void Free_Obstacles(struct Game *game){
//USUWANIE PRZESZKÓD LATAJĄCYCH Z LISTY//
    if(game->ofirst) {
        struct Obstacles *tmp;
        tmp=game->ofirst;
        game->oprev=NULL;
        while(tmp!=NULL){
            if(tmp->state==0){
                if(tmp==game->ofirst) {
                    game->ofirst=game->ofirst->next;
                    free(tmp);
                    tmp=game->ofirst;
                }
                else {
                    game->oprev->next=tmp->next;
                    free(tmp);
                    tmp=game->oprev->next;
                }
            }
            else {
                game->oprev=tmp;
                tmp=tmp->next;
            }
        }
    }
}
//==================================================== COLLIDE_OBSTACLES ========================================================//
void Collide_Obstacles(struct Game *game){
//SPRAWDZANIE KOLIZJI PRZESZKÓW ZE SMOKIEM//
    if(game->ofirst) {
        struct Obstacles *tmp;
        tmp=game->ofirst;
        while(tmp!=NULL && game->play.dragon_lives>0){
            if(tmp->state==1){   //mąka
                if((tmp->x1 + 20 < game->play.dragon_pos_x-70+85) && (tmp->x1 + 48 > game->play.dragon_pos_x-70) && (tmp->y1 + 20 < game->play.dragon_pos_y+90+25) && (tmp->y1 + 38 > game->play.dragon_pos_y+90)){
                    game->play.dragon_floured=true;
                    game->play.i3=0;
                    tmp->state=0;
                    if(rand() % 100 < 50){   game->play.in_hiss = al_create_sample_instance(game->play.hiss);   al_attach_sample_instance_to_mixer(game->play.in_hiss, game->mixer_sfx);    al_set_sample_instance_gain(game->play.in_hiss, game->sfxvol);   al_play_sample_instance(game->play.in_hiss);    }
                    else al_play_sample_instance(game->play.in_hiss);
                }
            }
            else if(tmp->state==2 || tmp->state==4){  //widły lub defragmenty
                if((tmp->x3 + 2 < game->play.dragon_pos_x-70+85) && (tmp->x3 + 7 > game->play.dragon_pos_x-70) && (tmp->y3 + 2 < game->play.dragon_pos_y+90+25) && (tmp->y3 + 7 > game->play.dragon_pos_y+90)  ){
                    tmp->state=0;
                    Revive_Dragon(game);
                    break;
                }
            }
            tmp=tmp->next;
        }
    }
}
//==================================================== ABSOLUTE_FREE ========================================================//
void Absolute_Free(struct Game *game){
//USUWANIE POCISKÓW//
    if(game->first) {
        struct Fire *tmp1;
        while(game->first){
            tmp1=game->first;
            game->first=game->first->next;
            free(tmp1);
        }
    game->first=NULL;
    game->prev=NULL;
    }
//USUWANIE DYMU//
    if(game->sfirst) {
        struct Smoke *tmp2;
        while(game->sfirst){
            tmp2=game->sfirst;
            game->sfirst=game->sfirst->next;
            free(tmp2);
        }
    game->sfirst=NULL;
    game->sprev=NULL;
    }
//USUWANIE WIEŚNIAKÓW//
    if(game->ffirst) {
        struct Fodder *tmp3;
        while(game->ffirst){
            tmp3=game->ffirst;
            game->ffirst=game->ffirst->next;
            free(tmp3);
        }
    game->ffirst=NULL;
    game->fprev=NULL;
    }
//USUWANIE PRZESZKÓD LATAJĄCYCH//
    if(game->ofirst) {
        struct Obstacles *tmp4;
        while(game->ofirst){
            tmp4=game->ofirst;
            game->ofirst=game->ofirst->next;
            free(tmp4);
        }
    game->ofirst=NULL;
    game->oprev=NULL;
    }
}
