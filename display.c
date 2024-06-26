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

//imprime um menu na tela e obtem inputs do usuario para realizar as operaçoes disponiveis em cada menu
bool display_menu(menus *m, display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, player *p1, player *p2, bool keys[]){

	//apaga todos os eventos em queue antes do menu ser aberto	
	al_flush_event_queue(queue);

	//para os ticks do timer enquanto o menu estiver aberto
	al_stop_timer(timer);


	ALLEGRO_EVENT event;
	ALLEGRO_FONT *font = al_create_builtin_font();
	ALLEGRO_COLOR color = al_map_rgb(0, 255, 255);
	ALLEGRO_COLOR color2 = al_map_rgb(0,0, 255);

	//controle de qual opcao do menu o usuario esta atualmente selecionando
	int atual = 0;

	//retorno da funcao
	bool retorno = false;

	//controle de continuacao do menu
	bool continua = true;
	while(continua){

		al_clear_to_color(al_map_rgb(0, 0 , 0));
		//imprime as opçoes do menu
		for(int i = 0; i < m->opcoes; i++){
			if(i == atual) //opçao atualmente selecionada
				al_draw_text(font, color2, disp->tam_x * 0.25, disp->tam_y * 0.25 + (i * disp->tam_y * 0.10), ALLEGRO_ALIGN_LEFT, m->strings[i]); 
			else //outras opçoes
				al_draw_text(font, color, disp->tam_x * 0.25, disp->tam_y * 0.25 + (i * disp->tam_y * 0.10), ALLEGRO_ALIGN_LEFT, m->strings[i]); 
		}

		//atualiza a tela
		al_flip_display();

		//espera e obtem input do usuario
		al_wait_for_event(queue, &event);
		if(event.type == 10){
			if(event.keyboard.keycode == 59) //pressiona 'esc' e fecha o menu de pause
				continua = false;
			else if (event.keyboard.keycode == 85) //pressiona o botao para baixo
				atual = (atual + 1) % m->opcoes;
			else if (event.keyboard.keycode == 84) //pressiona o botao para cima
				atual = (atual - 1 + m->opcoes) % m->opcoes;
			else if (event.keyboard.keycode == 67){//pressiona 'ENTER'
				if(m->codes[atual] == MAIN_MENU)
					retorno = main_menu(queue, disp, timer, true, p1, p2, keys);
				else if(m->codes[atual] == EXIT_GAME)
					retorno = true;
				continua = false;
			}
			keys[event.keyboard.keycode] = keys[event.keyboard.keycode] ^ 1;
		} else if (event.type == 12)
			keys[event.keyboard.keycode] = keys[event.keyboard.keycode] ^ 1;
	}

	//atualiza as teclas apertadas e soltas durante o menu
	while(!al_is_event_queue_empty(queue)){
		al_get_next_event(queue, &event);
		if(event.type == 10 || event.type == 12)
		keys[event.keyboard.keycode] = keys[event.keyboard.keycode] ^ 1;
	}

	//destoi a fonte
	al_destroy_font(font);

	//retoma os ticks do timer
	al_start_timer(timer);
	return retorno;
}

//imprime ambos os players na tela
void imprime_players(player *p1, player *p2){
	al_draw_rectangle(p1->x - p1->side / 2, p1->y - p1->height/2, p1->x + p1->side/2, p1->y + p1->height/2, al_map_rgb(0, 0, 255), 0);
	al_draw_rectangle(p2->x - p2->side / 2, p2->y - p2->height/2, p2->x + p2->side/2, p2->y + p2->height/2, al_map_rgb(255, 0, 0), 0);
	seleciona_sprite(p1);
	seleciona_sprite(p2);
	al_draw_scaled_bitmap(p1->sprite, 0, 0, p1->sprite_w, p1->sprite_h, p1->x - p1->side/2, p1->y - p1->height/2, p1->side, p1->height, ALLEGRO_MIN_LINEAR);
	al_draw_scaled_bitmap(p2->sprite, 0, 0, p2->sprite_w, p2->sprite_h, p2->x - p2->side/2, p2->y - p2->height/2, p2->side, p2->height, ALLEGRO_MIN_LINEAR);
}

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d){
	//destroi o display do allegro
	al_destroy_display(d->disp);
	
	//destroi a estrutura display_info
	free(d);
}
