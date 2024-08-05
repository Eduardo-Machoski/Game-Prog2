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
	new_display->chao = aux->height /7;
	new_display->full = true;

	//destroi o auxiliar
	free(aux);

	return new_display;
}

//torna o display full-screen se possivel (sem borda se borda = false, com borda se borda = true)
void full_screen(display_info *d, bool borda, ALLEGRO_EVENT_QUEUE *queue){

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

	//retira o registro do display destruido
	al_unregister_event_source(queue, al_get_display_event_source(d->disp));

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

	//registra o novo display na queue
	al_register_event_source(queue, al_get_display_event_source(d->disp));
}

//imprime um menu na tela e obtem inputs do usuario para realizar as operaçoes disponiveis em cada menu
bool display_menu(menus *m, display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single){

	//para os ticks do timer enquanto o menu estiver aberto
	al_stop_timer(timer);


	ALLEGRO_EVENT event;

	//controle de qual opcao do menu o usuario esta atualmente selecionando
	int atual = 0;

	//retorno da funcao
	bool retorno = false;

	//controle de continuacao do menu
	bool continua = true;
	
	//indica a sprite a ser aberta
	char *sprite = malloc(sizeof(char) * 1000);
	ALLEGRO_BITMAP *button = NULL;

	while(continua){
		if(background != NULL){
			imprime_background(background, disp);
			imprime_players(*p1, keys, false, true, 1);
			imprime_players(*p2, keys, false, true, 2);
		}
		//imprime as opçoes do menu
		for(int i = 0; i < m->opcoes; i++){
			sprite[0] = '\0';
			strcat(sprite, "Sprites/Menu/");
			if(i == atual){ //opçao atualmente selecionada
				strcat(sprite, "Collored_Buttons/");
				strcat(sprite, m->strings[i]);
			} else{
				strcat(sprite, "Buttons/");
				strcat(sprite, m->strings[i]);
			}

			//obtem a sprite do botao sendo impresso atualmente
			button = al_load_bitmap(sprite);
			if(!button){
				exit(1);
			}


			//imprime os botoes em escala
			if(m->codes[i] != BOSS)
				al_draw_scaled_bitmap(button, 0, 0, 600, 200, disp->tam_x/3, disp->tam_y/5 * (i + 1), disp->tam_x/3, disp->tam_y/6, ALLEGRO_MIN_LINEAR);	
			else
				al_draw_scaled_bitmap(button, 0, 0, 55, 57, 4 * disp->tam_x/9, disp->tam_y/5 * (i + 1), disp->tam_y/6, disp->tam_y/6, ALLEGRO_MIN_LINEAR);
			
			//destroi o bitmap do botao
			al_destroy_bitmap(button);
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
			else if (event.keyboard.keycode == 67)
			{//pressiona 'ENTER'
				if(m->codes[atual] == MAIN_MENU)
					retorno = main_menu(queue, disp, timer, true, p1, p2, keys, background, single);
				else if(m->codes[atual] == BOSS)
					*single = true;
				else if(m->codes[atual] == NEW_GAME || m->codes[atual] == START_GAME){
					retorno = selecao_personagem(disp, p1, p2, &background, queue, timer);
					*single = false;
				}else if(m->codes[atual] == EXIT_GAME)
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

	//destoi a string auxiliar sprite
	free(sprite);

	//retoma os ticks do timer
	al_start_timer(timer);
	return retorno;
}


//imprime um menu na tela e obtem inputs do usuario para realizar as operaçoes disponiveis em cada menu
bool display_menu_single(menus *m, display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, player *p, boss *b, bool keys[], ALLEGRO_BITMAP *background, bool *single){

	//para os ticks do timer enquanto o menu estiver aberto
	al_stop_timer(timer);


	ALLEGRO_EVENT event;

	//controle de qual opcao do menu o usuario esta atualmente selecionando
	int atual = 0;

	//retorno da funcao
	bool retorno = false;

	//controle de continuacao do menu
	bool continua = true;
	
	//indica a sprite a ser aberta
	char *sprite = malloc(sizeof(char) * 1000);
	ALLEGRO_BITMAP *button = NULL;

	while(continua){
		if(background != NULL){
			imprime_background(background, disp);
			imprime_players(p, keys, false, true, 1);
			imprime_boss(b, false);
		}
		//imprime as opçoes do menu
		for(int i = 0; i < m->opcoes; i++){
			sprite[0] = '\0';
			strcat(sprite, "Sprites/Menu/");
			if(i == atual){ //opçao atualmente selecionada
				strcat(sprite, "Collored_Buttons/");
				strcat(sprite, m->strings[i]);
			} else{
				strcat(sprite, "Buttons/");
				strcat(sprite, m->strings[i]);
			}

			//obtem a sprite do botao sendo impresso atualmente
			button = al_load_bitmap(sprite);
			if(!button){
				exit(1);
			}


			//imprime os botoes em escala
			if(m->codes[i] != BOSS)
				al_draw_scaled_bitmap(button, 0, 0, 600, 200, disp->tam_x/3, disp->tam_y/5 * (i + 1), disp->tam_x/3, disp->tam_y/6, ALLEGRO_MIN_LINEAR);	
			else
				al_draw_scaled_bitmap(button, 0, 0, 55, 57, 4 * disp->tam_x/9, disp->tam_y/5 * (i + 1), disp->tam_y/6, disp->tam_y/6, ALLEGRO_MIN_LINEAR);
			
			//destroi o bitmap do botao
			al_destroy_bitmap(button);
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
			else if (event.keyboard.keycode == 67)
			{//pressiona 'ENTER'
				if(m->codes[atual] == MAIN_MENU){
					retorno = false;
					*single = false;
				}
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

	//destoi a string auxiliar sprite
	free(sprite);

	//retoma os ticks do timer
	al_start_timer(timer);
	return retorno;
}
//imprime ambos os players na tela
void imprime_players(player *p1, bool *keys, bool hitbox, bool pause, int player){
	//player nao inicializado
	if(!p1)
		return;

	if(hitbox){
	//imprime hitbox
		al_draw_rectangle(p1->x - p1->side / 2, p1->y - p1->height/2, p1->x + p1->side/2, p1->y + p1->height/2, al_map_rgb(0, 0, 255), 0);

		//hitbox attack_1 p1
		if((p1->attack == 1)){
			if(p1->olha_esquerda)
				al_draw_rectangle(p1->x - p1->side/2 - p1->attack_1[0], p1->y - p1->attack_1[1]/2, p1->x - p1->side/2, p1->y + p1->attack_1[1]/2, al_map_rgb(255,0,0), 1);
			else
				al_draw_rectangle(p1->x + p1->side/2 + p1->attack_1[0], p1->y - p1->attack_1[1]/2, p1->x + p1->side/2, p1->y + p1->attack_1[1]/2, al_map_rgb(255,0,0), 1);
		}

		//hitbox attack_2 p1
		if((p1->attack == 2)){
			if(p1->olha_esquerda)
				al_draw_rectangle(p1->x - p1->side/2 - p1->attack_2[0], p1->y - p1->attack_2[1]/2, p1->x - p1->side/2, p1->y + p1->attack_2[1]/2, al_map_rgb(255,0,0), 1);
			else
				al_draw_rectangle(p1->x + p1->side/2 + p1->attack_2[0], p1->y - p1->attack_2[1]/2, p1->x + p1->side/2, p1->y + p1->attack_2[1]/2, al_map_rgb(255,0,0), 1);
		}

	}

	//verifica se o jogo nao esta pausado para mudar a sprite
	if(!pause)
		//seleciona a sprite correta a ser impressa nesse ciclo
		seleciona_sprite(p1, player, keys);

	//verifica qual a orientacao do player (esquerda ou direita)
	orientacao_player(p1, keys, player);

	//imprime os players
	if(p1->olha_esquerda)
		al_draw_scaled_bitmap(p1->sprite, 0, 0, p1->sprite_w, p1->sprite_h, p1->x - p1->side_sprite/2, p1->y - p1->height_sprite/2, p1->side_sprite, p1->height_sprite, ALLEGRO_MIN_LINEAR ^ ALLEGRO_FLIP_HORIZONTAL);	
	else
		al_draw_scaled_bitmap(p1->sprite, 0, 0, p1->sprite_w, p1->sprite_h, p1->x - p1->side_sprite/2, p1->y - p1->height_sprite/2, p1->side_sprite, p1->height_sprite, ALLEGRO_MIN_LINEAR);
}


//impirme o boss do single player
void imprime_boss(boss *b, bool hitbox){
	if(hitbox)
	//imprime hitbox
		al_draw_rectangle(b->x - b->side / 2, b->y - b->height/2, b->x + b->side/2, b->y + b->height/2, al_map_rgb(0, 0, 255), 0);
	
	/*
	//verifica se o jogo nao esta pausado para mudar a sprite
	if(!pause)
		//seleciona a sprite correta a ser impressa nesse ciclo
		seleciona_sprite(p1, player, keys);
*/

	//imprime os players
	if(b->olha_esquerda)
		al_draw_scaled_bitmap(b->sprite[0], 0, 0, b->sprite_w, b->sprite_h, b->x - b->side_sprite/2, b->y - b->height_sprite/2, b->side_sprite, b->height_sprite, ALLEGRO_MIN_LINEAR ^ ALLEGRO_FLIP_HORIZONTAL);	
	else
		al_draw_scaled_bitmap(b->sprite[0], 0, 0, b->sprite_w, b->sprite_h, b->x - b->side_sprite/2, b->y - b->height_sprite/2, b->side_sprite, b->height_sprite, ALLEGRO_MIN_LINEAR);
}

//imprime o background selecionado (tamanho da sprite 3440x1440 pixels)
void imprime_background(ALLEGRO_BITMAP *background, display_info *disp){
	al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, disp->tam_x, disp->tam_y, ALLEGRO_MIN_LINEAR);
}

//imprime a barra de stamina de ambos os players
//aumenta a stamina atual dos players a cada 2 ciclos do timer
void imprime_stamina(display_info *disp, player *p1, player *p2, bool adiciona){
	
	//valores utilizados na impressao (melhora performance)
	int disp_3 = disp->tam_x/3;
	int disp_15 = disp->tam_x/10;
	int disp_12_y = disp->tam_y/12 * 2 - disp->tam_y/25;
	int disp_30_y = disp->tam_y/30 + disp->tam_y/12 - disp->tam_y/50;

	//stamina do player1
	al_draw_filled_rectangle(disp_15 - 2, disp_30_y, disp_3 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(disp_15, disp_30_y + 1, disp_15 + (disp_3 - disp_15) * p1->stamina/100.0, disp_12_y - 1, al_map_rgb(255, 127, 80));

	//stamina do player2
	al_draw_filled_rectangle(2 * disp_3 - 2, disp_30_y, disp->tam_x - disp_15 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(2 * disp_3 + (disp ->tam_x - disp_15 - 2 * disp_3) * (1.0 - p2->stamina/100.0), disp_30_y + 1, disp->tam_x - disp_15, disp_12_y - 1, al_map_rgb(255, 127, 80));


	//aumenta um pouco a stamina do player caso nao esteja cheia
	if(adiciona && p1->stamina < 100)
		p1->stamina += 1;
	if(adiciona && p2->stamina < 100)
		p2->stamina += 1;

}

//imprime a barra de vida de ambos os players
void imprime_vida(display_info *disp, player *p1, player *p2){

	//valores utilizados na impressao (melhora performance)
	int disp_3 = disp->tam_x/3;
	int disp_15 = disp->tam_x/15;
	int disp_12_y = disp->tam_y/12;
	int disp_30_y = disp->tam_y/30;

	//vida do player1
	al_draw_filled_rectangle(disp_15 - 2, disp_30_y, disp_3 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(disp_15, disp_30_y + 1, disp_15 + (disp_3 - disp_15) * p1->vida/100.0, disp_12_y - 1, al_map_rgb(0, 0, 255));

	//vida do player2
	al_draw_filled_rectangle(2 * disp_3 - 2, disp_30_y, disp->tam_x - disp_15 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(2 * disp_3 + (disp ->tam_x - disp_15 - 2 * disp_3) * (1.0 - p2->vida/100.0), disp_30_y + 1, disp->tam_x - disp_15, disp_12_y - 1, al_map_rgb(255, 0, 0));
}

//imprime a barra de vida de ambos os players
void imprime_vida_boss(display_info *disp, boss *b){

	//valores utilizados na impressao (melhora performance)
	int disp_3 = disp->tam_x/3;
	int disp_15 = disp->tam_x/15;
	int disp_12_y = disp->tam_y/12;
	int disp_30_y = disp->tam_y/30;

	//vida do boss
	al_draw_filled_rectangle(2 * disp_3 - 2, disp_30_y, disp->tam_x - disp_15 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(2 * disp_3 + (disp ->tam_x - disp_15 - 2 * disp_3) * (1.0 - b->vida/100.0), disp_30_y + 1, disp->tam_x - disp_15, disp_12_y - 1, al_map_rgb(255, 0, 0));
}

//imprime a barra de vida do player1
void imprime_vida_single(display_info *disp, player *p1){

	//valores utilizados na impressao (melhora performance)
	int disp_3 = disp->tam_x/3;
	int disp_15 = disp->tam_x/15;
	int disp_12_y = disp->tam_y/12;
	int disp_30_y = disp->tam_y/30;

	//vida do player1
	al_draw_filled_rectangle(disp_15 - 2, disp_30_y, disp_3 + 2, disp_12_y, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(disp_15, disp_30_y + 1, disp_15 + (disp_3 - disp_15) * p1->vida/100.0, disp_12_y - 1, al_map_rgb(0, 0, 255));
}

void imprime_selecao(display_info *disp, int p1, int p2, int backi, ALLEGRO_BITMAP *background){
	imprime_background(background, disp);

	//bitmaps auxiliares
	ALLEGRO_BITMAP *vermelho = al_load_bitmap("Sprites/Selection/vermelho.png");
	ALLEGRO_BITMAP *azul = al_load_bitmap("Sprites/Selection/azul.png");
	ALLEGRO_BITMAP *branco = al_load_bitmap("Sprites/Selection/branco.png");

	int w = al_get_bitmap_width(vermelho);
      	int h = al_get_bitmap_height(vermelho);	
	//subbitmaps com a parte do bitmap que sera impressa
	ALLEGRO_BITMAP *sub_vermelho = al_create_sub_bitmap(vermelho, 3 * w/5, 3 * h/4, w/5, h/4);
	ALLEGRO_BITMAP *sub_branco = al_create_sub_bitmap(branco, 3 * w/5, 3 * h/4, w/5, h/4);
	ALLEGRO_BITMAP *sub_azul = al_create_sub_bitmap(azul, 3 * w/5, 3 * h/4, w/5, h/4);
	
	ALLEGRO_BITMAP *atual;

	//imprime a selecao do player 1
	for(int i = 0; i < 4; i++){
		if(i == p1)
			atual = sub_vermelho;
		else
			atual = sub_branco;
		int inicial = disp->tam_x/4 + (i * disp->tam_x/6 - disp->tam_x/12);
		al_draw_scaled_bitmap(atual, 0, 0, al_get_bitmap_width(atual), al_get_bitmap_height(atual), inicial, disp->tam_y/4, disp->tam_x/6, disp->tam_y/4, 0); 
	}

	//imprime a selecao do player 2
	for(int i = 0; i < 4; i++){
		if(i == p2)
			atual = sub_azul;
		else
			atual = sub_branco;
		int inicial = disp->tam_x/4 + (i * disp->tam_x/6 - disp->tam_x/12);
		al_draw_scaled_bitmap(atual, 0, 0, al_get_bitmap_width(atual), al_get_bitmap_height(atual), inicial, 2 * disp->tam_y/4, disp->tam_x/6, disp->tam_y/4, 0); 
	}
	//destoi os subbitmaps
	al_destroy_bitmap(sub_vermelho);
	al_destroy_bitmap(sub_azul);
	al_destroy_bitmap(sub_branco);

	//destroi os bitmaps auxiliares
	al_destroy_bitmap(vermelho);
	al_destroy_bitmap(branco);
	al_destroy_bitmap(azul);

	//cria bitmaps com os icones dos herois
	ALLEGRO_BITMAP *fantasy = al_load_bitmap("Sprites/Selection/fantasy_warrior.png");	
	ALLEGRO_BITMAP *hero_1 = al_load_bitmap("Sprites/Selection/martial_hero.png");	
	ALLEGRO_BITMAP *hero_2 = al_load_bitmap("Sprites/Selection/martial_hero_3.png");	
	ALLEGRO_BITMAP *warrior = al_load_bitmap("Sprites/Selection/medieval_warrior.png");	
	
	//imprime o fantasy warrior
	al_draw_scaled_bitmap(fantasy, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	al_draw_scaled_bitmap(fantasy, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18, 2 * disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);

	//imprime o heroi 1
	al_draw_scaled_bitmap(hero_1, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/6, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	al_draw_scaled_bitmap(hero_1, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/6, 2 * disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	
	//imprime o heroi 2
	al_draw_scaled_bitmap(hero_2, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/3, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	al_draw_scaled_bitmap(hero_2, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/3, 2 * disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	
	//imprime o warrior
	al_draw_scaled_bitmap(warrior, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/2, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	al_draw_scaled_bitmap(warrior, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/2, 2 * disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);

	//detroi os bitmaps dos icones dos herois
	al_destroy_bitmap(fantasy);
	al_destroy_bitmap(hero_1);
	al_destroy_bitmap(hero_2);
	al_destroy_bitmap(warrior);

	//atualiza a tela
	al_flip_display();
}

//imprime a selecao de personagem no single player
void imprime_selecao_single(display_info *disp, int p1, int backi, ALLEGRO_BITMAP *background){
	imprime_background(background, disp);

	//bitmaps auxiliares
	ALLEGRO_BITMAP *vermelho = al_load_bitmap("Sprites/Selection/vermelho.png");
	ALLEGRO_BITMAP *branco = al_load_bitmap("Sprites/Selection/branco.png");

	int w = al_get_bitmap_width(vermelho);
      	int h = al_get_bitmap_height(vermelho);	
	//subbitmaps com a parte do bitmap que sera impressa
	ALLEGRO_BITMAP *sub_vermelho = al_create_sub_bitmap(vermelho, 3 * w/5, 3 * h/4, w/5, h/4);
	ALLEGRO_BITMAP *sub_branco = al_create_sub_bitmap(branco, 3 * w/5, 3 * h/4, w/5, h/4);
	
	ALLEGRO_BITMAP *atual;

	//imprime a selecao do player
	for(int i = 0; i < 4; i++){
		if(i == p1)
			atual = sub_vermelho;
		else
			atual = sub_branco;
		int inicial = disp->tam_x/4 + (i * disp->tam_x/6 - disp->tam_x/12);
		al_draw_scaled_bitmap(atual, 0, 0, al_get_bitmap_width(atual), al_get_bitmap_height(atual), inicial, disp->tam_y/4, disp->tam_x/6, disp->tam_y/4, 0); 
	}

	//destoi os subbitmaps
	al_destroy_bitmap(sub_vermelho);
	al_destroy_bitmap(sub_branco);

	//destroi os bitmaps auxiliares
	al_destroy_bitmap(vermelho);
	al_destroy_bitmap(branco);

	//cria bitmaps com os icones dos herois
	ALLEGRO_BITMAP *fantasy = al_load_bitmap("Sprites/Selection/fantasy_warrior.png");	
	ALLEGRO_BITMAP *hero_1 = al_load_bitmap("Sprites/Selection/martial_hero.png");	
	ALLEGRO_BITMAP *hero_2 = al_load_bitmap("Sprites/Selection/martial_hero_3.png");	
	ALLEGRO_BITMAP *warrior = al_load_bitmap("Sprites/Selection/medieval_warrior.png");	
	
	//imprime o fantasy warrior
	al_draw_scaled_bitmap(fantasy, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);

	//imprime o heroi 1
	al_draw_scaled_bitmap(hero_1, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/6, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	
	//imprime o heroi 2
	al_draw_scaled_bitmap(hero_2, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/3, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);
	
	//imprime o warrior
	al_draw_scaled_bitmap(warrior, 0, 0, al_get_bitmap_width(fantasy), al_get_bitmap_height(fantasy), disp->tam_x/4 - disp->tam_x/18 + disp->tam_x/2, disp->tam_y/4 + disp->tam_y/24, disp->tam_x/8, disp->tam_y/6, 0);

	//detroi os bitmaps dos icones dos herois
	al_destroy_bitmap(fantasy);
	al_destroy_bitmap(hero_1);
	al_destroy_bitmap(hero_2);
	al_destroy_bitmap(warrior);

	//atualiza a tela
	al_flip_display();
}
void imprime_score(int n1, int n2, display_info *disp, ALLEGRO_FONT *font){

	//cria o texto do score do player1
	char score[2];
	score[0] = n1 + 48;
	score[1] = '\0';

	//imprime o score do player1
	al_draw_text(font, al_map_rgb(0, 0, 255), disp->tam_x/3 + 20, disp->tam_y/19, ALLEGRO_ALIGN_LEFT, score);


	//arruma o texto do score para o do player 2
	score[0] = n2 + 48;

	//imprime o score do player2
	al_draw_text(font, al_map_rgb(255, 0, 0), 2 * disp->tam_x/3 - 20, disp->tam_y/19, ALLEGRO_ALIGN_RIGHT, score);
}

//realiza a animacao de morte do player derrotado(p1) e idle do outro(p2)
////imprime o texto indicando qual o ganhador
void animacao_morte(player *p1, player *p2, display_info *disp, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP *background, char num){
	ALLEGRO_EVENT event;

	//animacao de morte do derrotado
	p1->sprite_atual = p1->i_sprites[8];
	p1->tempo_ciclo = 0;

	//animacao idle do ganhador
	p2->sprite_atual = 0;
	p2->tempo_ciclo = 0;

	//numero de sprites na animacao idle do ganhador
	int aux = p2->i_sprites[1];

	//cor do texto de vitoria de um player
	ALLEGRO_COLOR color;
	if(num == '2')
		color = al_map_rgb(255, 0, 0);
	else
		color = al_map_rgb(0, 0, 255);

	char text[15] = "Player   Wins!";
	text[7] = num;

	//encerra o ciclo quando acaha a animacao de morte do perdedor
	while(p1->sprite_atual < p1->num_sprites){
		al_wait_for_event(queue, &event);

		if(event.type == 30){

			//imprime o background
			imprime_background(background, disp);

			//imprime o texto indicando o vencedor
			al_draw_text(font, color, disp->tam_x/2, disp->tam_y/2, ALLEGRO_ALIGN_CENTRE, text);

			//imprime os players
			if(p1->olha_esquerda)
				al_draw_scaled_bitmap(p1->sprite, 0, 0, p1->sprite_w, p1->sprite_h, p1->x - p1->side_sprite/2, p1->y - p1->height_sprite/2, p1->side_sprite, p1->height_sprite, ALLEGRO_MIN_LINEAR ^ ALLEGRO_FLIP_HORIZONTAL);	
			else
				al_draw_scaled_bitmap(p1->sprite, 0, 0, p1->sprite_w, p1->sprite_h, p1->x - p1->side_sprite/2, p1->y - p1->height_sprite/2, p1->side_sprite, p1->height_sprite, ALLEGRO_MIN_LINEAR);
			if(p2->olha_esquerda)
				al_draw_scaled_bitmap(p2->sprite, 0, 0, p2->sprite_w, p2->sprite_h, p2->x - p2->side_sprite/2, p2->y - p2->height_sprite/2, p2->side_sprite, p2->height_sprite, ALLEGRO_MIN_LINEAR ^ ALLEGRO_FLIP_HORIZONTAL);
			else
				al_draw_scaled_bitmap(p2->sprite, 0, 0, p2->sprite_w, p2->sprite_h, p2->x - p2->side_sprite/2, p2->y - p2->height_sprite/2, p2->side_sprite, p2->height_sprite, ALLEGRO_MIN_LINEAR);

			al_flip_display();

			//anda na animacao de morte do perdedor	
			if(p1->tempo_ciclo >= p1->frames * 2){
				p1->sprite_atual = p1->sprite_atual + 1;
				p1->tempo_ciclo = 0;

				//atualiza o subbitmap
				al_destroy_bitmap(p1->sprite);
				p1->sprite = al_create_sub_bitmap(p1->bitmap, p1->sprite_w * p1->sprite_atual, 0, p1->sprite_w * (p1->sprite_atual + 1), p1->sprite_h);
			} else
				p1->tempo_ciclo += 1;

			//anda na animacao idle do ganhador
			if(p2->tempo_ciclo >= p2->frames * 2){
				p2->sprite_atual = (p2->sprite_atual + 1) % aux;
				p2->tempo_ciclo = 0;

				//atualzia o subbitmap
				al_destroy_bitmap(p2->sprite);
				p2->sprite = al_create_sub_bitmap(p2->bitmap, p2->sprite_w * p2->sprite_atual, 0, p2->sprite_w * (p2->sprite_atual + 1), p2->sprite_h);
			} else
				p2->tempo_ciclo += 1;

		}
	}
} 

//destroi um display_info e todos os seus componentes
void destroy_display_info(display_info *d){
	//destroi o display do allegro
	al_destroy_display(d->disp);
	
	//destroi a estrutura display_info
	free(d);
}
