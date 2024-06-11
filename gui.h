#ifndef __GUI__
#define __GUI__

typedef struct {
	int opcoes;
	int *codes;
	int atual;
	char **strings;	
} menus;

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>

#include"player.h"
#include"display.h"

#define RETURN_GAME 0
#define MAIN_MENU 1


//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam);

//pausa o jogo, remove todos os innputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, display_info *disp);

//imprime ambos os players na tela
void imprime_players(player *p1, player *p2);

//destroi um menu e seus componentes
void destroy_menu(menus *m);
#endif
