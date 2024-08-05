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
#include"single.h"

#define RETURN_GAME 0
#define MAIN_MENU 1
#define EXIT_GAME 2
#define START_GAME 3
#define NEW_GAME 4
#define BOSS 5


//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam);

//menu principal do jogo
bool main_menu(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, bool reset, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single);

//selecao de personagens e background
bool selecao_personagem(display_info *disp, player **p1, player **p2, ALLEGRO_BITMAP **background, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);

//selecao do heroi e do background no single player
bool selecao_single(display_info *disp, player **p1, ALLEGRO_BITMAP **background, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);

//pausa o jogo, remove todos os innputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single);

//Diz quem foi o ganhador, realiza animacao de morte do perdedor e verifica se quer jogar outra, retornar ao menu
//ou sair do jogo
bool tela_vitoria(player **p1, player **p2, display_info *disp, int num, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP **background, bool keys[], ALLEGRO_FONT *font, bool *single);


//destroi um menu e seus componentes
void destroy_menu(menus *m);
#endif
