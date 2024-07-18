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
#define EXIT_GAME 2
#define START_GAME 3


//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam);

//menu principal do jogo
	bool main_menu(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, bool reset, player *p1, player *p2, bool keys[]);

void selecao_personagem(player *p1, player *p2);

//pausa o jogo, remove todos os innputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, player *p1, player *p2, bool keys[]);


//destroi um menu e seus componentes
void destroy_menu(menus *m);
#endif
