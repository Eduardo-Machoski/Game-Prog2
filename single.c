#include"single.h"

//main do modo single player
bool single_player(display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP **background, bool keys[], player **p, bool *single, int dev_mode){
	
	//player seleciona o heroi e o background
	bool retorno = selecao_single(disp, p, background, queue, timer);

	//verifica se entra no loop do single player ou encerra o programa
	bool encerra = retorno;
	bool adiciona_stamina = true;

	ALLEGRO_EVENT event;

	//cria o boss que o player ira enfrentar
	boss *inimigo = cria_boss(disp);

	int code;
	while(!encerra){

		//aguarda e obtem o proximo evento, assim como seu codigo
		al_wait_for_event(queue, &event);
		code = event.type;

		//tick do timer
		if(code == 30){
			move_player_single(*p , disp, keys);
			imprime_background(*background, disp);
			imprime_stamina_single(disp, *p, adiciona_stamina);
			adiciona_stamina = adiciona_stamina ^ 1;
			imprime_vida_single(disp, *p);
			imprime_players(*p, keys, dev_mode, false, 1);
			al_flip_display();
		} else if(code == 10){ //tecla pressionada
			//atualiza o controle de movimento dos personagens
			keys[event.keyboard.keycode] = 1;

			//pressiona 'f11' para obter tela full-screen
			if(event.keyboard.keycode == 57){
				al_unregister_event_source(queue, al_get_display_event_source(disp->disp));
				if(disp->full)
					full_screen(disp, true, queue);
				else
					full_screen(disp, false, queue);
				al_register_event_source(queue, al_get_display_event_source(disp->disp));
			}

			/*
			//pressiona 'esc' para pausar o jogo e abrir o menu de pausa
			if(event.keyboard.keycode == 59)
				encerra = pause_gui(queue, disp, timer, &player_1, &player_2, pressed_keys, background, &single);
*/
		} else if(code == 12){ // tecla liberada
			//atualiza o controle de movimento dos personagens
			keys[event.keyboard.keycode] = 0;
		} else if(code == 42) //botao de fechar pressionado
			encerra = true;
 	}

	destroy_boss(inimigo);
	
	return retorno;
}

boss *cria_boss(display_info *disp){
        //cria a estrutura e verifica a alocação de memoria
        boss *aux;
        if(!(aux = malloc(sizeof(boss))))
               exit(1);
	
	//caminho para as sprites do boss
	char source_boss[] = "Sprites/Boss/sprite.png";

	//caminhos para as sprites de cada ataque
	char source_bola[] = "Sprites/Boss/bola.png";
	char source_olho[] = "Sprites/Boss/olho.png";
	char source_coluna[] = "Sprites/Boss/coluna.png";

        //vetor de bitmap para as sprites
        ALLEGRO_BITMAP **bitmap = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
	if(!bitmap)
		exit(1);

	//load de todas as sprites
        if(!(bitmap[0] = al_load_bitmap(source_boss)))
                exit(1);
        if(!(bitmap[1] = al_load_bitmap(source_bola)))
                exit(1);
        if(!(bitmap[2] = al_load_bitmap(source_olho)))
                exit(1);
        if(!(bitmap[3] = al_load_bitmap(source_coluna)))
                exit(1);

        //abre a file com informações das sprites
        FILE *file;
        char source_info[] = "Sprites/Boss/info.txt"; 
	if(!(file = fopen(source_info, "r")))
                exit(1);

        //numero de sprites de animação do boss e cada ataque
        int *num_sprites;
	if(!(num_sprites = malloc(sizeof(int) * 4)))
		exit(1);
	for(int i = 0; i < SPRITES_BOSS; i++)
        	fscanf(file, "%d", &num_sprites[i]);

        //cria o subbitmap que ira conter apenas uma sprite
        ALLEGRO_BITMAP **sprite = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
	if(!sprite)
		exit(1);

	//valores auxiliares
	int width = al_get_bitmap_width(bitmap[0])/num_sprites[0];
	int height = al_get_bitmap_height(bitmap[0]);

	//cria os subbitmaps de cada bitmap
	for(int i = 0; i < SPRITES_BOSS; i++)
        	if(!(sprite[i] = al_create_sub_bitmap(bitmap[i], 0, 0, al_get_bitmap_width(bitmap[i])/num_sprites[i], al_get_bitmap_height(bitmap[i]))))
                	exit(1);

        //dimensoes do corpo do personagem
        int tam_x, tam_y;
        fscanf(file, "%d %d", &tam_x, &tam_y);

        //indices onde cada sequencia de animacao comeca
        int *indice_sprites;
        if(!(indice_sprites = malloc(sizeof(int) * 5)))
                exit(1);
        for(int i = 0; i < 5; i++)
                fscanf(file, "%d", &indice_sprites[i]);


	int *sprite_atual;
	if(!(sprite_atual = calloc(0, sizeof(int) * SPRITES_BOSS)))
		exit(1);

	int *tempo_ciclo;
	if(!(tempo_ciclo = calloc(0, sizeof(int) * SPRITES_BOSS)))
		exit(1);

        //inicializa o boss
        aux->side = disp->tam_x / 12;
        aux->side_sprite = (disp->tam_x / 12) * (width/tam_x);
        aux->height = disp->tam_y / 3;
        aux->height_sprite = (disp->tam_y / 3) * (height/tam_y);
        aux->vida = 100;
        aux->bitmap = bitmap;
        aux->sprite = sprite;
        aux->sprite_atual = sprite_atual;
        aux->sprite_w = width;
        aux->sprite_h = height;
        aux->olha_esquerda = true;
        aux->num_sprites = num_sprites;
        aux->i_sprites = indice_sprites;
        aux->attack = 0;
        aux->attack_done = true;
        aux->tempo_ciclo = tempo_ciclo;

        //posicao horizontal inicial (ini_x% da tela)
       aux->x = disp->tam_x * (90.0/100.0);

        //inicializa os players no chao)
        aux->y = disp->tam_y - disp->chao - aux->height/2;
        
       	//fecha a file de informacao
        fclose(file);
        return aux;
}


//destroi a struct do boss e todos os seus componentes
void destroy_boss(boss *elem){
	for(int i = 0; i < SPRITES_BOSS; i++){
		//destroi os bitmaps de animacao e ataques do boss
		al_destroy_bitmap(elem->bitmap[i]);

		//destroi os subbitmaps do boss
		al_destroy_bitmap(elem->sprite[i]);
	}

	//destroi os vetores dentro da struct
	free(elem->i_sprites);
	free(elem->sprite_atual);
	free(elem->tempo_ciclo);
	free(elem->num_sprites);

	//destroi a struct
	free(elem);

}
