#ifndef __GUI__
#define __GUI__

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>

#include"player.h"

//pausa o jogo, remove todos os innputs ainda nao processados
void pause_gui(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);

//imprime ambos os players na tela
void imprime_players(player *p1, player *p2);

#endif
