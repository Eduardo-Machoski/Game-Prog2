#ifndef __PLAYER__
#define __PLAYER__

#include<stdlib.h>
#include<stdbool.h>
#include<allegro5/allegro.h>

#include"display.h"

#define VELOCIDADE 5

typedef struct {
	int x;
	int y;
	int side;
	int vida;
} player;

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(int tam, int ini_x, int ini_y);

//atualiza a posicao x e y dos players a partir dos seus controles
void move_players(player *p1, player *p2, display_info *disp, bool *keys);

//destroi um player e seus componentes
void destroy_player(player *elem);

#endif
