#include"single.h"

//main do modo single player
bool single_player(display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP **background, ALLEGRO_FONT *font, bool keys[], player **p, bool *single, int dev_mode){
	
	//player seleciona o heroi e o background
	bool retorno = selecao_single(disp, p, background, queue, timer);

	//verifica se entra no loop do single player ou encerra o programa
	bool encerra = retorno;

	bool verifica = true;

	ALLEGRO_EVENT event;

	//cria o boss que o player ira enfrentar
	boss *inimigo = cria_boss(disp);
	ataque *attack = cria_ataque();

	player *aux = *p;

	srand(time(NULL));

	int code;
	int ciclo_atual = 200;
	while(!encerra){

		//aguarda e obtem o proximo evento, assim como seu codigo
		al_wait_for_event(queue, &event);
		code = event.type;

		//tick do timer
		if(code == 30){
			aux->stamina = 100;
			//movimenta o player a partir dos inputs
			move_player_single(*p , disp, keys);
			
			//verifica se o boss realiza um ataque, se sim verifica se ha hit no player
			//verifica se o ataque ja acabou
			ataques_boss(*p, keys, dev_mode, inimigo, attack, disp);

			if(attack->code == BOLA)
				andar_ataque(attack, *p);
			else if(attack->code == COLUNA && verifica)
				verifica = verifica_hit(attack, *p, inimigo);
			
			//atualiza a sprite que deve ser impressa do boss, seu estado atual e posicao
			movimento_boss(inimigo, &ciclo_atual, disp);

			//verifica se o player realiza um ataque, se sim verifica se ha hit no boss
			//verifica se o ataque ja acabou
			ataque_player(*p, keys, dev_mode, inimigo);

			//imprime o background
			imprime_background(*background, disp);

			//imprime a vida do player
			imprime_vida_single(disp, *p);

			//imprime a vida do boss
			imprime_vida_boss(disp, inimigo);

			//imprime ataque
			imprime_ataque(attack, inimigo, dev_mode);

			//imprime o player
			imprime_players(*p, keys, dev_mode, false, 1);

			//imprime o boss
			imprime_boss(inimigo, dev_mode);

			al_flip_display();
			
			//verifica se chegou no momento do boss atacar
			if(ciclo_atual == 300){
				//inimigo->attack = 1 + rand() % 2;
				inimigo->attack = 1;
				if(inimigo->attack == 2)
					verifica = true;
				ciclo_atual = 0;
			} else if (inimigo->attack == 0)
				ciclo_atual += 1;

			//verifica se o player ganhou ou perdeu
			encerra = verifica_fim(*p, inimigo, disp, timer, queue, *background, keys, font, &retorno, single);
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

		} else if(code == 12){ // tecla liberada
			//atualiza o controle de movimento dos personagens
			keys[event.keyboard.keycode] = 0;
		} else if(code == 42){ //botao de fechar pressionado
			encerra = true;
			retorno = true;
		}
 	}

	//destroi as struct especificas do single player
	destroy_ataque(attack);
	destroy_boss(inimigo);
	
	return retorno;
}


//movimenta o bos (tanto animaçoes, quanto o estado de ataque)
void movimento_boss(boss *b, int *ciclo, display_info *disp){
	
	//reposiciona o boss conforme seu estado
	if(b->attack == COLUNA){
		b->x = disp->tam_x/2;
		b->y = disp->tam_y/3;
	} else if(b->attack == BOLA){
		b->x = disp->tam_x * 0.9;
		b->y = disp->tam_y - disp->chao - b->height/2;
	} else{
		b->x = disp->tam_x/2;
		b->y = disp->tam_y - disp->chao - b->height/2;
	}
	
	int tempo_ciclo;

	if(b->attack == 1)
		tempo_ciclo = 3;
	else
		tempo_ciclo = 8;

	if(b->attack != 0){
		//proximo bitmap no ciclo dos ataques
		if(b->tempo_ciclo[b->attack] >= tempo_ciclo){
			b->tempo_ciclo[b->attack] = 0;
			b->sprite_atual[b->attack] = (b->sprite_atual[b->attack] + 1) % b->num_sprites[b->attack];
			int width = al_get_bitmap_width(b->sprite[b->attack]);
			al_destroy_bitmap(b->sprite[b->attack]);
			b->sprite[b->attack] = al_create_sub_bitmap(b->bitmap[b->attack], width * b->sprite_atual[b->attack], 0, width, al_get_bitmap_height(b->bitmap[b->attack]));
			if(!b->sprite[b->attack])
				exit(10);
		} else
			b->tempo_ciclo[b->attack] += 1;
	}

	int num = status_boss(b);
	bool troca = false;
	
	//verifica se o estado (jump, idle...) mudou
        if(!(b->sprite_atual[0] >= b->i_sprites[num] && (num == 4 || b->sprite_atual[0] < b->i_sprites[num + 1]))){
                b->sprite_atual[0] = b->i_sprites[num];
                b->tempo_ciclo[0] = 0;
                troca = true;
        }

	
	//verifica se é necessario mudar a imagem por conta do tempo
        if(b->tempo_ciclo[0] >= 5){
                int num_sprites;
                if(num < 4)
                        num_sprites = b->i_sprites[num + 1] - b->i_sprites[num];
                else
                        num_sprites = b->num_sprites[0] - b->i_sprites[num];

                //troca o indice da imagem (depende de qual animaçao esta ocorrendo no momento)
                b->sprite_atual[0] = b->i_sprites[num] + ((b->sprite_atual[0] - b->i_sprites[num] + 1) % num_sprites);
                troca = true;

                //reinicia o tempo do ciclo atual
                b->tempo_ciclo[0] = 0;
        } else{
                //acrescenta tempo no ciclo atual
                b->tempo_ciclo[0] += 1;
        }

	if(b->sprite_atual[0] == b->i_sprites[4] - 1 && b->tempo_ciclo[0] == 4){
		b->recuo = false;
		b->sprite_atual[0] = 0;
	}
        //cria o subbitmap com a sprite selecionada para impressao do boss
        if(troca){
                al_destroy_bitmap(b->sprite[0]);
                b->sprite[0] = al_create_sub_bitmap(b->bitmap[0], b->sprite_w * b->sprite_atual[0], 0, b->sprite_w, b->sprite_h);
                if(!b->sprite[0])
                        exit(1);
        }

}

//verifica o status do boss e retona qual a animacao ele deve realiza no momento
int status_boss(boss *b){
	int retorno = 0;

	if(b->attack == 0 && b->recuo) //boss tomou hit
		retorno = 3;
	else if(b->attack == COLUNA) //boss realizando o attack de coluna
		retorno = 1;
	else if(b->attack == BOLA) //boss realizando o attack de bola
		retorno = 2;

	return retorno;
}

//verifica se o player realiza um ataque, se sim verifica se ha hit no boss
//verifica se o ataque ja acabou
void ataque_player(player *p, bool keys[], bool dev_mode, boss *b){
	if(!p->jump && !p->crouch  && !p->recuo && p->attack == 0){ //verifica se o p1 pode atacar
                if(keys[ALLEGRO_KEY_Z]){//ataque alto
                        p->attack = 1;
                        p->attack_done = false;
                }
                if(keys[ALLEGRO_KEY_X]){//ataque baixo
                        p->attack = 2;
                        p->attack_done = false;
                }
        } 
	
	int num;
	//verifica hits dos ataques do p1
        if(p->attack == 1){
		num = 6;
	} else if (p->attack == 2){
		num = 7;
	} else
		return;
	
	if((p->sprite_atual < p->i_sprites[num] - 1) || b->recuo || b->attack != 0)
                return;

        //indica se houve um hit
        bool acerto = false;

        //atacando para esquerda
        if(p->olha_esquerda && (b->x < p->x) && (b->x + b->side/2 > p->x - p->side/2 - p->attack_1[0]))
                acerto = true;
        else if(!p->olha_esquerda && (b->x > p->x) && (b->x - b->side/2 < p->x + p->side/2 + p->attack_1[0])) //atacando para direita
                acerto = true;

        if(acerto){
                b->vida -= 5 * p->attack;
                b->recuo = true;
                b->attack = 0;
	}

}

//verifica se o boss realiza um ataque, se sim verifica se ha hit no player
//verifica se o ataque ja acabou
void ataques_boss(player *p, bool keys[], bool hitbox, boss *b, ataque *attack, display_info *disp){
	
	//verifica se o boss esta realizando um ataque
	if(b->attack == 0)
		return;

	//identifia o ataque atual
	switch(b->attack){
		case(BOLA)://bola de fogo
			if(b->attack_resta == 0)
				b->attack_resta = 300 + rand() % 901;//tempo que ficara invocando bolas de fogo
			else
				b->attack_resta -= 1;
			if(b->attack_resta % 75 == 0){//invoca uma nova bola de fogo
				if(rand() % 2 == 0)//define se vai ser uma bola alta ou baixa
					adiciona_ataque(attack, disp->tam_x * 0.8, b->y - disp->tam_y/8, disp->tam_x/10, disp->tam_x/6, false, true, BOLA);
				else
					adiciona_ataque(attack, disp->tam_x * 0.8, b->y, disp->tam_x/10, disp->tam_x/6, true, false, BOLA);
			}
		break;
		case(COLUNA): //colunas de fogo
			if(b->attack_resta == 0){//tempo do ataque de colunas
				b->attack_resta = 88;
				for(int i = 0; i < 5; i++)//cria 5 colunas de fogo
					adiciona_ataque(attack, disp->tam_x/7 + disp->tam_x/6 * i, disp->tam_y/6, disp->tam_x/14, 2 * disp->tam_y, false, false, COLUNA); 
			} else
				b->attack_resta -= 1;
		break;
		default:
		break;
	}

	if(b->attack_resta == 0){//fim do ataque
		b->attack = 0;
		attack->num = 0;
		for(int i = 0; i < 6; i++)
			attack->valido[i] = false;
		attack->code = 0;
	}
}


//verifica se o player ganhou ou perdeu o modo single player
//realiza o encerramento dependendo se ganhou ou perdeu
//reinicia o boss e o player se new game
//sai do modo single player se main menu ou exit
bool verifica_fim(player *p, boss *b, display_info *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP *background, bool keys[], ALLEGRO_FONT *font, bool *retorno, bool *single){

        //realiza a animacao de morte do player derrotadoou do boss 
        //imprime o texto indicando se o player venceu ou perdeu
	if(p->vida == 0)
		single_derrota(p, b, disp, font, queue, background);
	else if(b->vida == 0)
		single_vitoria(p, b, disp, font, queue, background);
	else
		return false;
        
	//cria o menu que sera usado no display
        menus *m;
        if(!(m = cria_menu(3)))
                return false;

        //cria as strings do menu
        char fala1[] = "new_game.png";
        char fala2[] = "menu.png";
        char fala3[] = "quit.png";

        //inicializa as strings do menu
        m->strings[0] = fala1;
        m->strings[1] = fala2;
        m->strings[2] = fala3;

        //inicializa os codigos
        m->codes[0] = NEW_GAME;
        m->codes[1] = MAIN_MENU;
        m->codes[2] = EXIT_GAME;

        //mostra o menu e realiza a operacao desejada pelo usuario
        *retorno = display_menu_single(m, disp, queue, timer, p, b, keys, background, single);


        //destroi o menu apos o seu uso
        destroy_menu(m);

        return true;
}
//adiciona um ataque na struct ataque
void adiciona_ataque(ataque *attack, int x, int y, int tam_x, int tam_y, bool low, bool high, int code){

	if(attack->num == 0){
		//atribui os valores constantes
		attack->code = code;
		attack->tam_x = tam_x;
		attack->tam_y = tam_y;
	}
	
	int num = attack->num % 6;

	//atribui os valores dos vetores
	attack->pos_x[num] = x;
	attack->pos_y[num] = y;
	attack->low[num] = low;
	attack->high[num] = high;
	attack->valido[num] = true;

	attack->num += 1;
	if(attack->num > 6)
		attack->num = 6;
}


//cria a struct que controla os ataques do boss
ataque *cria_ataque(){

	//aloca a struct na memoria
	ataque *aux;
	if(!(aux = malloc(sizeof(ataque))))
		exit(1);	

	//cria os vetores da struct
	int *pos_x;
	int *pos_y;
	bool *low;
	bool *high;
	bool *valido;
	if(!(pos_x = malloc(sizeof(int) * 6)))
		exit(1);
	if(!(pos_y = malloc(sizeof(int) * 6)))
		exit(1);
	if(!(low = malloc(sizeof(bool) * 6)))
		exit(1);
	if(!(high = malloc(sizeof(bool) * 6)))
		exit(1);
	if(!(valido = malloc(sizeof(bool) * 6)))
		exit(1);

	//inicializa os vetores
	for(int i = 0; i < 6; i++){
		pos_x[i] = 0;
		pos_y[i] = 0;
		low[i] = false;
		high[i] = false;
		valido[i] = false;
	}

	//inicializa a struct
	aux->num = 0;
	aux->pos_x = pos_x;
	aux->pos_y = pos_y;
	aux->low = low;
	aux->high = high;
	aux->valido = valido;
	aux->tam_x = 0;
	aux->tam_y = 0;
	aux->code = 0;

	return aux;
}

//'anda' a bola de fogo em direcao a esquerda e verifica hit no player
void andar_ataque(ataque *attack, player *p){
	for(int i = 0; i < attack->num; i++){
		if(attack->valido[i]){
			attack->pos_x[i] -= 5 * VELOCIDADE_X/3;
			if(attack->pos_x[i] - attack->tam_x/2 < p->x + p->side/2 && attack->pos_x[i] > p->x - p->side/2){
				if((attack->low[i] && !p->jump) || (attack->high[i] && !p->crouch)){
					p->recuo = true;
					p->attack = 0;
					p->sprite_atual = p->i_sprites[7];
					p->tempo_ciclo = 0;
					p->vida -= 15;
					if(p->vida < 0)
						p->vida = 0;
					attack->valido[i] = false;
				}
			}
			if(attack->pos_x[i] - attack->tam_x/2 <= 0)
				attack->valido[i] = false;
		}
	}
}

//verifica se o player foi atinjigo pelas colunas de fogo
bool verifica_hit(ataque *attack, player *p, boss *b){
	bool acertou = true;
	 for(int i = 0; i < attack->num; i++){//verifica para cada coluna
                if(attack->valido[i] && b->sprite_atual[COLUNA] >= 4){//coluna valida e no tempo do ataque
                        if(attack->pos_x[i] - attack->tam_x/2 < p->x + p->side/2 && attack->pos_x[i] + attack->tam_x/2 > p->x - p->side/2 && !p->recuo){//verifica colisao do hitbox
				p->recuo = true;
				p->attack = 0;
				p->sprite_atual = p->i_sprites[7];
				p->tempo_ciclo = 0;
				p->vida -= 25;
				if(p->vida < 0)
					p->vida = 0;
				acertou = false;
                        }

			//impede o player de tomar dano das colunas de fogo nesse ataque novamente
                        if(attack->pos_x[i] - attack->tam_x/2 <= 0)
                                attack->valido[i] = false;
                }
        }

	 return acertou;
}

//destroi a struct ataque e todos os seus componentes
void destroy_ataque(ataque *elem){
	free(elem->pos_x);
	free(elem->pos_y);
	free(elem->low);
	free(elem->high);
	free(elem->valido);
	free(elem);
}


boss *cria_boss(display_info *disp){
        //cria a estrutura e verifica a alocação de memoria
        boss *aux;
        if(!(aux = malloc(sizeof(boss))))
               exit(1);
	
        //vetor de bitmap para as sprites
        ALLEGRO_BITMAP **bitmap = malloc(sizeof(ALLEGRO_BITMAP*) * 3);
	if(!bitmap)
		exit(1);

	//load de todas as sprites
        if(!(bitmap[0] = al_load_bitmap("Sprites/Boss/sprite.png")))
                exit(1);
        if(!(bitmap[1] = al_load_bitmap("Sprites/Boss/bola.png")))
                exit(1);
        if(!(bitmap[2] = al_load_bitmap("Sprites/Boss/coluna.png")))
                exit(1);

        //abre a file com informações das sprites
        FILE *file;
        char source_info[] = "Sprites/Boss/info.txt"; 
	if(!(file = fopen(source_info, "r")))
                exit(1);

        //numero de sprites de animação do boss e cada ataque
        int *num_sprites;
	if(!(num_sprites = malloc(sizeof(int) * 3)))
		exit(1);
	for(int i = 0; i < SPRITES_BOSS; i++)
        	fscanf(file, "%d", &num_sprites[i]);

        //cria o subbitmap que ira conter apenas uma sprite
        ALLEGRO_BITMAP **sprite = malloc(sizeof(ALLEGRO_BITMAP*) * 3);
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
	if(!(sprite_atual = calloc(SPRITES_BOSS, sizeof(int))))
		exit(1);

	int *tempo_ciclo;
	if(!(tempo_ciclo = calloc(SPRITES_BOSS, sizeof(int))))
		exit(1);

        //inicializa o boss
        aux->side = disp->tam_x / 16;
        aux->side_sprite = (disp->tam_x / 16) * (width/tam_x);
        aux->height = disp->tam_y / 4;
        aux->height_sprite = (disp->tam_y / 4) * (height/tam_y);
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
        aux->tempo_ciclo = tempo_ciclo;
	aux->attack_resta = 0;
	aux->recuo = false;

        //posicao horizontal inicial (ini_x% da tela)
       aux->x = disp->tam_x/2;

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
