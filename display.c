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
	new_display->full = false;

	return new_display;
}

//torna o display full-screen se possivel (sem borda se borda = false, com borda se borda = true)
void full_screen(display_info *d, bool borda){

	//cria a estrutura auxiliar ara verificar o tamanho da tela
	ALLEGRO_DISPLAY_MODE *aux;
	//verifica se o allocamento de memoria foi bem sucedido
	if(!(aux = malloc(sizeof(ALLEGRO_DISPLAY_MODE))))
		return;

	//obtem a informacao do tamanho da tela
       	al_get_display_mode(0, aux);
	//verifica se a informacao foi obtida corretamente
	if(!aux)
		return;

	//display com borda para display fullscreen com borda
	if(borda && !d->full){
		//atualiza o tamanho do display e de atualiza suas informacoes
		al_resize_display(d->disp, aux->width, aux->height);
		d->tam_x = aux->width;
		d->tam_y = aux->height;
		d->chao = d->tam_y / 4;

		//faz o drive grafico atualizar o tamanho da tela
		al_acknowledge_resize(d->disp);
		return;
	}

	if(borda)
		al_set_new_display_flags(ALLEGRO_WINDOWED); //display com borda
	else
		al_set_new_display_flags(ALLEGRO_FULLSCREEN); //display sem borda

	
	//cria um novo display e atualiza suas flags para fullscreen sem borda
	ALLEGRO_DISPLAY *aux2;
       	if(!(aux2 = al_create_display(aux->width, aux->height)))
		return;

	//destroi o display antigo
	al_destroy_display(d->disp);

	//atualiza a estrutura que agora guarda o novo display
	d->disp = aux2;
	d->tam_x = aux->width;
	d->tam_y = aux->height;
	d->full = !borda;

	//atualiza o display
	al_flip_display();
}

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d){
	//destroi o display do allegro
	al_destroy_display(d->disp);
	
	//destroi a estrutura display_info
	free(d);
}
