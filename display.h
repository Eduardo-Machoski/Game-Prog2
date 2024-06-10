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

//cria um novo display de tamanho tamx x tam_y
//cria uma estrutura auxiliar para guardar informacoes do display
//retorna um ponteiro para a estrutura auxiliar (display_info*)
display_info *cria_display();

//torana o display full-screen se possivel
//attualiza o display e suas informacoes com os novos valores
void full_screen(display_info *d, bool borda);

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d);

#endif
