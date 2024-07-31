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
	bool main_menu(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, bool reset, player *p1, player *p2, bool keys[], ALLEGRO_BITMAP *background);

//selecao de personagens e background
bool selecao_personagem(display_info *disp, player **p1, player **p2, ALLEGRO_BITMAP **background, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);

//pausa o jogo, remove todos os innputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, player *p1, player *p2, bool keys[], ALLEGRO_BITMAP *background);


//destroi um menu e seus componentes
void destroy_menu(menus *m);
#endif
