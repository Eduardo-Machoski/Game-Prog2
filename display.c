#include"display.h"

//cria um novo display de tamanha tam_x x tam_y e armazena suas informacoes na estrutura display_info
display_info *cria_display(int tam_x, int tam_y){

	//alloca espaco em memoria para a estrutura display_info
	display_info *new_display;
	if(!(new_display = malloc(sizeof(display_info))))
		exit(1);
	
	//cria o novo display
	new_display->disp = al_create_display(tam_x, tam_y);

	//inicializa os valores da estrutura
	new_display->tam_x = tam_x;
	new_display->tam_y = tam_y;
	new_display->chao = tam_y /4;

	return new_display;
}

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d){
	//destroi o display do allegro
	al_destroy_display(d->disp);
	
	//destroi a estrutura display_info
	free(d);
}
