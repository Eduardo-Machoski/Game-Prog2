#include"display.h"

//cria um novo display de tamanha tam_x x tam_y e armazena suas informacoes na estrutura display_info
display_info *cria_display(){

	//alloca espaco em memoria para a estrutura display_info
	display_info *new_display;
	if(!(new_display = malloc(sizeof(display_info))))
		exit(1);

	//estrutura do allegro pra obter informacoes da tela
	ALLEGRO_DISPLAY_MODE *aux;
	if(!(aux = malloc(sizeof(ALLEGRO_DISPLAY_MODE))))
		exit(1);

	//obtem as informacoes da tela
	al_get_display_mode(0, aux);
	//verifica se a informacao foi obtida corretamente
	if(!aux)
		exit(2);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);

	//cria o novo display
	new_display->disp = al_create_display(aux->width, aux->height);

	//inicializa os valores da estrutura
	new_display->tam_x = aux->width;
	new_display->tam_y = aux->height;
	new_display->chao = aux->height /4;
	new_display->full = true;

	//destroi o auxiliar
	free(aux);

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

		//destroi o auxiliar
		free(aux);

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

	//destroi o auxiliar
	free(aux);

	//atualiza o display
	al_flip_display();
}

bool display_menu(menus *m, display_info *disp, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue){
	bool continua = true;
	ALLEGRO_EVENT event;
	while(continua){
		for(int i = 0; i < m->opcoes; i++){
			al_draw_text(font, al_map_rgb(0, 255, 255), disp->tam_x * 0.25, disp->tam_y * 0.25 + (i * disp->tam_y * 0.10), ALLEGRO_ALIGN_LEFT, m->strings[i]); 
		}
		al_flip_display();
		al_wait_for_event(queue, &event);
		if(event.type == 10 && event.keyboard.keycode == 59){
			al_flush_event_queue(queue);
			return false;
		}
	}
	return true;	
}

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d){
	//destroi o display do allegro
	al_destroy_display(d->disp);
	
	//destroi a estrutura display_info
	free(d);
}
