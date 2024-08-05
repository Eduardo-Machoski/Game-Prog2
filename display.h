#ifndef __DISPLAY__
#define __DISPLAY__

#include<allegro5/allegro.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>

typedef struct {
	ALLEGRO_DISPLAY *disp;
	int tam_x;
	int tam_y;
	int chao;
	bool full;
} display_info;

#include"gui.h"
#include"single.h"

//cria um novo display de tamanho tamx x tam_y
//cria uma estrutura auxiliar para guardar informacoes do display
//retorna um ponteiro para a estrutura auxiliar (display_info*)
display_info *cria_display();

//torana o display full-screen se possivel
//attualiza o display e suas informacoes com os novos valores
void full_screen(display_info *d, bool borda, ALLEGRO_EVENT_QUEUE *queue);

//imprime um menu na tela e obtem inputs do usuario para realizar as operaçoes disponiveis em cada menu
bool display_menu(menus *m, display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single);

//imprime ambos os players na tela
void imprime_players(player *p1, bool *keys, bool hitbox, bool pause, int player);

//imprime o boss do single player
void imprime_boss(boss *b, bool hitbox);

//imprime o background selecionado (tamanho da sprite 3440x1440 pixels)
void imprime_background(ALLEGRO_BITMAP *background, display_info *disp);

//imprime a barra de stamina de ambos os players
//aumenta a stamina atual dos players a cada 2 ciclos do timer 
void imprime_stamina(display_info *disp, player *p1, player *p2, bool adiciona);

//imprime a barra de stamina do player1
//aumenta a stamina atual dos players a cada 2 ciclos do timer 
void imprime_stamina_single(display_info *disp, player *p1, bool adiciona);

//imprime a barra de vida de ambos os players
void imprime_vida(display_info *disp, player *p1, player *p2);

//imprime a barra de vida do player1
void imprime_vida_single(display_info *disp, player *p1);

//imprime a tela de selecao e qual a opcao atual dos players (baseado em p1 e p2)
void imprime_selecao(display_info *disp, int p1, int p2, int back, ALLEGRO_BITMAP *background);

//imprime a tela de selecao no single player e qual a opcao atual dos players (baseado em p1)
void imprime_selecao_single(display_info *disp, int p1, int back, ALLEGRO_BITMAP *background);

//imprime o score dos players (ex: 0 x 0)
void imprime_score(int n1, int n2, display_info *disp, ALLEGRO_FONT *font);

//realiza a animacao de morte do player derrotado(p1) e idle do outro(p2)
//imprime o texto indicando qual o ganhador
void animacao_morte(player *p1, player *p2, display_info *disp, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP *background, char num);

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d);

#endif
