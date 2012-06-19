#ifndef PLAY_H_INCLUDED
#define PLAY_H_INCLUDED
#include "main.h"

void Play_Game(struct Game *game); //rysowanie wszystkich grafik podczas gry, inkrementowanie wartoœci np. pozycji poszczególnych elementów z list
void Increment_All(struct Game *game); //inkrementowanie ró¿nych zmiennych u¿ywanych w czasie trwania gry
void Revive_Dragon(struct Game *game); //utrata ¿ycia przez smoka, wywo³uje te¿ funkcjê zwolnienia wszystkich list - nowe ¿ycie, nowa szansa

#endif // PLAY_H_INCLUDED
