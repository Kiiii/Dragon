#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

//==================================================== FIRE_BULLET ========================================================//
void Fire_Bullet(struct Game *game)    {
    struct Fire *tmp = (struct Fire *)malloc (sizeof(struct Fire));
    tmp->exist = 1;
    tmp->fire_pos_x = game->play.dragon_pos_x+5;
    tmp->fire_pos_y = game->play.dragon_pos_y+95;
    tmp->fire_pos_x2 = game->pos_x;
    tmp->fire_pos_y2 = game->pos_y;
    tmp->a=((tmp->fire_pos_y - tmp->fire_pos_y2)/(tmp->fire_pos_x - tmp->fire_pos_x2));
    tmp->b=(tmp->fire_pos_y - tmp->a * tmp->fire_pos_x);
    tmp->x = tmp->fire_pos_x;
    tmp->y=(((tmp->a)*(tmp->x))+(tmp->b));
    tmp->next = NULL;
    if(game->first==NULL){
        game->first=tmp;
        game->last=tmp;
    }
    else {
        game->last->next=tmp;
        game->last=tmp;
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
            else if (tmp==game->last && tmp!=game->first){
                game->last=game->prev;
                free(tmp);
                tmp=NULL;
                game->last->next=NULL;
            }
            else{
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
    struct Fodder *tmp = (struct Fodder *)malloc (sizeof(struct Fodder));
    tmp->type = ((rand() % 5 == 4)? (/*3 +*/ rand() % 4) : (rand() % 5)); //zmniejszenie prawdopodobieństwa wystąpienia domków
    tmp->lives=((tmp->type >2)? 3 : 1); //przydział punktów życia w zależnoœci od typu foddera
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

    if(game->ffirst==NULL){
        game->ffirst=tmp;
        game->flast=tmp;
    }
    else {
        game->flast->next=tmp;
        game->flast=tmp;
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
             else if (tmp==game->flast && tmp!=game->ffirst){
                game->flast=game->fprev;
                free(tmp);
                tmp=NULL;
                game->flast->next=NULL;
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
//SPRAWDZA KOLIZJE POCISKÓW Z WIEŚNIAKAMI//
    struct Fire *tmp;
    tmp=game->first;

    struct Fodder *ftmp;
    ftmp=game->ffirst;

    while(ftmp!=NULL){
        tmp=game->first;
        while(tmp!=NULL){
            if((tmp->exist==1) && (ftmp->lives>0) && (tmp->x >= (ftmp->x1-30)) && (tmp->x <= (ftmp->x2+30)) && (tmp->y >= ftmp->y1-30) && (tmp->y <= ftmp->y2+30) ){
                ftmp->lives-=1;
                if(ftmp->type==2 && ftmp->lives==0)     game->play.score_sheeps+=2;
                else if((ftmp->type==3 || ftmp->type==4) && ftmp->lives==0)     game->play.score_huts+=3;
                else if(ftmp->lives==0)     game->play.score_villagers++;
             // printf("%p\t%p\t%p\n",ftmp,game->ffirst,ftmp);
                tmp->exist=2;
                break;
            }
            //printf("%p\t%p\t%p\n",tmp,game->first,ftmp);
            tmp=tmp->next;
        }
       // printf("%p\t%p - wieśniak\n",ftmp,game->ffirst);
        ftmp=ftmp->next;
    }
}
//==================================================== ADD_SMOKE ========================================================//
void Add_Smoke(struct Game *game, float x1, float y1) {
    struct Smoke *tmp = (struct Smoke *)malloc (sizeof(struct Smoke));
    tmp->frame_x=15;
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
    struct Obstacles *tmp = (struct Obstacles *)malloc (sizeof(struct Obstacles));
    tmp->state=s;
    tmp->x1=x1;
    tmp->y1=y1;
    tmp->x3=tmp->x1;
    if(tmp->state==2){
        tmp->a=((y2 - tmp->y1)/(x2 - tmp->x1));
        tmp->b=(tmp->y1 - ((y2 - tmp->y1)/(x2 - tmp->x1))*tmp->x1);
        tmp->y3=tmp->a*tmp->x3+tmp->b;
    }
    tmp->frame_y=0;
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
    if(game->ofirst) {
        struct Obstacles *tmp;
        tmp=game->ofirst;
        game->oprev=NULL;
        while(tmp!=NULL){
            if((tmp->state!=1 && tmp->state!=2) || tmp->x1<-50 || tmp->y1<-40 || tmp->x3<-50) {
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
    if(game->ofirst) {
        struct Obstacles *tmp;
        tmp=game->ofirst;
        while(tmp!=NULL && game->play.dragon_lives>0){
        //printf("%p : %d\n",tmp,tmp->state);
         if(tmp->state==1){                                     //mąka
            if((tmp->x1 + 20 < game->play.dragon_pos_x-70+85) &&
               (tmp->x1 + 48 > game->play.dragon_pos_x-70) &&
               (tmp->y1 + 20 < game->play.dragon_pos_y+90+25) &&
               (tmp->y1 + 38 > game->play.dragon_pos_y+90)  ){
                game->play.dragon_floured=true;
                game->play.i3=0;
                tmp->state=0;
            }
         }
         else if(tmp->state==2){                                //widły
            if((tmp->x3 + 2 < game->play.dragon_pos_x-70+85) &&
               (tmp->x3 + 7 > game->play.dragon_pos_x-70) &&
               (tmp->y3 + 2 < game->play.dragon_pos_y+90+25) &&
               (tmp->y3 + 7 > game->play.dragon_pos_y+90)  ){
                //tmp->state=0; //to musi być przed Revive, bo tam był Absolute Free, a potem tmp->state chciało zmieniać i były śmieci
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
    game->last=NULL;
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
    game->flast=NULL;
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
