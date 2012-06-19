#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED //L_H_INCLUDED
#include "main.h"
#include "play.h"

void Fire_Bullet(struct Game *game); //wystrzelenie pocisku - dodaje element do listy jednokierunkowej
void Free_Fire(struct Game *game); //usuniêcie pocisku z listy, gdy spe³niony bêdzie dany warunek

void Create_Fodder(struct Game *game); //pojawianie siê wieœniaków - dodanie elementu do listy
void Free_Fodder(struct Game *game); //usuniêcie elementu z listy, gdy wyjdzie za ekran lub zginie
void Burn_Fodder(struct Game *game); //sprawdzanie kolizji - dla ka¿dego wieœniaka sprawdza wszystkie pociski

void Add_Smoke(struct Game *game, float x, float y, int type); //dodanie dymku od pocisku do listy
void Free_Smoke(struct Game *game); //usuniêcie elementu z listy, gdy wszystkie klatki jego animacji przelec¹

void Add_Obstacles(struct Game *game, float x1, float y1, int state, float x2, float y2); //dodanie przeszkód lataj¹cych do listy - wid³e³, chmur m¹ki, owczych defragmentatorów
void Free_Obstacles(struct Game *game); //usuniêcie przeszków z listy po kolizji ze smokiem lub wyleceniu za ekran
void Collide_Obstacles(struct Game *game); //dla ka¿dego elementu sprawdza, czy jego wspó³rzêdne wchodz¹ w obszar smoka

void Absolute_Free(struct Game *game); //zwolnienie wszystkich elementów ze wszystkich list - NEW GAME albo strata ¿ycia

#endif // LISTS_H_INCLUDED
