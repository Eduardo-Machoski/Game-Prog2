#ifndef __PLAYER__
#define __PLAYER__

#include<stdlib.h>
#include<stdbool.h>
#include<allegro5/allegro.h>

#include"display.h"

#define VELOCIDADE_X 10 
#define VELOCIDADE_MAX_Y 0.09

typedef struct {
	int x;
	int y;
	int side;
	int height;
	int vida;
	bool jump;
	float jump_height;
} player;

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int x_ini);

//atualiza a posicao x e y dos players a partir dos seus controles
void move_players(player *p1, player *p2, display_info *disp, bool *keys);

//destroi um player e seus componentes
void destroy_player(player *elem);

#endif
