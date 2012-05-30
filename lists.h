#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED //L_H_INCLUDED
#include "main.h"
#include "play.h"

void Fire_Bullet(struct Game *game);
void Add_Smoke(struct Game *game, float x, float y);
void Free_Smoke(struct Game *game);
void Create_Fodder(struct Game *game);
void Burn_Fodder(struct Game *game);
void Free_Fire(struct Game *game);
void Free_Fodder(struct Game *game);

void Add_Obstacles(struct Game *game, float x1, float y1, int state, float x2, float y2);
void Free_Obstacles(struct Game *game);
void Collide_Obstacles(struct Game *game);

void Absolute_Free(struct Game *game);

#endif // LISTS_H_INCLUDED
