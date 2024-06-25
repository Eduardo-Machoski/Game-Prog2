#ifndef __DiSPLAY__
#define __DiSPLAY__

#include<allegro5/allegro.h>

typedef struct {
	ALLEGRO_DISPLAY *disp;
	int tam_x;
	int tam_y;
	int chao;
	bool full;
} display_info;

#include"gui.h"

//cria um novo display de tamanho tamx x tam_y
//cria uma estrutura auxiliar para guardar informacoes do display
//retorna um ponteiro para a estrutura auxiliar (display_info*)
display_info *cria_display();

//torana o display full-screen se possivel
//attualiza o display e suas informacoes com os novos valores
void full_screen(display_info *d, bool borda);

//imprime um menu na tela e obtem inputs do usuario para realizar as operaçoes disponiveis em cada menu
bool display_menu(menus *m, display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, player *p1, player *p2, bool keys[]);

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d);

#endif
