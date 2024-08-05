#include"player.h"

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int ini_x, bool esquerda, char *pasta){

	//cria a estrutura e verifica a alocação de memoria
	player *aux;
        if(!(aux = malloc(sizeof(player))))
	       exit(1);

	char *bitmap_source = malloc(sizeof(char) * 100);
	bitmap_source[0] = '\0';

	strcat(bitmap_source, pasta);
	strcat(bitmap_source, "sprite.png");

	//bitmap para as sprites
	ALLEGRO_BITMAP *bitmap = NULL;
	if(!(bitmap = al_load_bitmap(bitmap_source)))
		exit(1);

	free(bitmap_source);

	//abre a file com informações das sprites
	FILE *file;
	strcat(pasta, "info.txt");
	if(!(file = fopen(pasta, "r")))
		exit(1);

	//numero de sprites de animação
	int num_sprites;
	fscanf(file, "%d", &num_sprites);

	//dimençoes do subbitmap
	int width = al_get_bitmap_width(bitmap)/num_sprites;
	int height = al_get_bitmap_height(bitmap);

	//cria o subbitmap que ira conter apenas uma sprite
	ALLEGRO_BITMAP *sprite = NULL;
	if(!(sprite = al_create_sub_bitmap(bitmap, 0, 0, width, height)))
		exit(1);

	//dimensoes do corpo do personagem
	int tam_x, tam_y;
	fscanf(file, "%d %d", &tam_x, &tam_y);

	//indices onde cada sequencia de animacao comeca
	int *indice_sprites;
	if(!(indice_sprites = malloc(sizeof(int) * 9)))
		exit(1);
	for(int i = 0; i < 9; i++)
		fscanf(file, "%d", &indice_sprites[i]);

	int frames;
	fscanf(file, "%d", &frames);

	//tamanhos do hitbox de cada ataque
	int *attack_1, *attack_2;
	if(!(attack_1 = malloc(sizeof(int) * 2)))
		exit(1);

	if(!(attack_2 = malloc(sizeof(int) * 2)))
		exit(1);

	//le o hitbox em relacao a sprite do heroi
	fscanf(file, "%d %d", &attack_1[0], &attack_1[1]);
	fscanf(file, "%d %d", &attack_2[0], &attack_2[1]);

	//ajusta as hitboxes para o tamanho da tela atual
	attack_1[0] = (disp->tam_x/8) * (attack_1[0]/(float)width);
	attack_1[1] = (disp->tam_x/8) * (attack_1[1]/(float)width);

	attack_2[0] = (disp->tam_x/8) * (attack_2[0]/(float)width);
	attack_2[1] = (disp->tam_x/8) * (attack_2[1]/(float)width);

	//inicializa o player
	aux->side = disp->tam_x / 16;
	aux->side_sprite = (disp->tam_x / 16) * (width/tam_x);	
	aux->height = disp->tam_y / 4;
	aux->height_sprite = (disp->tam_y / 4) * (height/tam_y);
	aux->stamina = 100;
	aux->vida = 100;
	aux->jump = false;
	aux->jump_height = VELOCIDADE_MAX_Y;
	aux->bitmap = bitmap;
	aux->sprite = sprite;
	aux->sprite_atual = 0;
	aux->sprite_w = width;
	aux->sprite_h = height;
	aux->olha_esquerda = esquerda;
	aux->num_sprites = num_sprites;
	aux->i_sprites = indice_sprites;
	aux->attack = 0;
	aux->recuo = false;
	aux->crouch = false;
	aux->attack_done = true;
	aux->frames = frames;
	aux->attack_1 = attack_1;
	aux->attack_2 = attack_2;
	aux->tempo_ciclo = 0;
	aux->vitorias = 0;

	//posicao horizontal inicial (ini_x% da tela)
	aux->x = disp->tam_x * (ini_x/100.0);

	//inicializa os players no chao)
	aux->y = disp->tam_y - disp->chao - aux->height/2;

	//fecha a file de informacao
	fclose(file);
	return aux;
}

//verifica se o player pode atacar no momento, se sim atualiza o p->attack para indicar isso
void verifica_ataque(player *p1, player *p2, bool *keys, display_info *disp){
	if(!p1->jump && !p1->crouch  && !p1->recuo && p1->attack == 0){ //verifica se o p1 pode atacar
		if(keys[ALLEGRO_KEY_Z] && p1->stamina >= 30){//verifica stamina para ataque alto
			p1->attack = 1;
			p1->attack_done = false;
		}
		if(keys[ALLEGRO_KEY_X] && p1->stamina >= 10){//verifica stamina para ataque baixo
			p1->attack = 2;
			p1->attack_done = false;
		}
	}

	if(!p2->jump && !p2->crouch && !p2->recuo && p2->attack == 0){//verifica se o p2 pode atacar
		if(keys[ALLEGRO_KEY_PAD_1] && p2->stamina >= 30){//verifica stamina para ataque alto 
			p2->attack = 1;
			p2->attack_done = false;
		}
		if(keys[ALLEGRO_KEY_PAD_2] && p2->stamina >= 10){//verifica stamina para ataque baixo
			p2->attack = 2;
			p2->attack_done = false;
		}
	}

	//verifica hits dos ataques do p1
	if(p1->attack == 1)
		attack_1(p1, p2);
	else if(p1->attack == 2)
		attack_2(p1, p2);

	//verifica se o p1 venceu o round
	if(p2->vida <= 0){
		p2->vida = 0;
		reset_round(p1, p2, disp);
	}

	//verifica hits dos ataques do p2
	if(p2->attack == 1)
		attack_1(p2, p1);
	else if (p2->attack == 2)
		attack_2(p2, p1);

	//verifica se o p2 venceu o round
	if(p1->vida <= 0){
		p1->vida = 0;
		reset_round(p2, p1, disp);
	}
}

//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, display_info  *disp, bool *keys){

	int half_side_p1 = p1->side/2;
	int half_side_p2 = p2->side/2;

	//divisor da velocidade (1 velocidade normal, 2 velociadade diminuida)
	int crouch_1 = 1;
	int crouch_2 = 1;


	//verifica se o player1 esta agachado e atualiza a velociadade
	if(p1->attack == 0 && !p1->jump  && !p1->recuo && keys[ALLEGRO_KEY_S]){
		p1->crouch = true;
		crouch_1 = 2;
	} else{
		p1->crouch = false;
	}

	//verifica se o player2 esta agachado e atualiza a velociadade
	if(p2->attack == 0 && !p2->jump && !p1->recuo && keys[ALLEGRO_KEY_DOWN]){
		p2->crouch = true;
		crouch_2 = 2;
	} else{
		p2->crouch = false;
	}
	
	if(p1->attack == 0 && !p1->recuo){
		//atualiza a posicao x do player1
		p1->x += (VELOCIDADE_X/crouch_1) * (keys[4] - keys[1]);
		//verifica colisao do player1 com a borda da tela nas laterais
		if(p1->x < half_side_p1 || p1->x > disp->tam_x - half_side_p1)
			p1->x = half_side_p1 * keys[1] + (disp->tam_x - half_side_p1) * keys[4];
	}

	if(p2->attack == 0 && !p1->recuo){	
		//atualiza a posicao x do player2
		p2->x += (VELOCIDADE_X/crouch_2) * (keys[83] - keys[82]);
		//verifica colisao do player2 com a borda da tela nas lateraiss
		if(p2->x < half_side_p2 || p2->x > disp->tam_x - half_side_p2)
			p2->x = half_side_p2 * keys[82] + (disp->tam_x - half_side_p2) * keys[83];
	}

	//verifica se o p1 esta saltando
	if(p1->jump){
		
		//atualiza posicao vertical de p1
		p1->y -= disp->tam_y * p1->jump_height;

		//verifica se o pulo foi encerrado
		if(p1->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p1->jump_height = VELOCIDADE_MAX_Y;
			p1->y = disp->tam_y - (p1->height/2 + disp->chao);
			p1->jump = false;
			p1->stamina -= 10;
		}

		//atualiza o valor do salto para efeito da gravidade
		p1->jump_height -= 0.01;
	} else if(keys[23] && p1->attack  == 0 && !p1->recuo && p1->y == disp->tam_y - (p1->height/2 + disp->chao) && p1->stamina >= 10)//p1 em posicao valida para saltar
		p1->jump = true;
	

	//verifica se o p2 esta saltando
	if(p2->jump){

		//atualiza posicao vertical de p2
		p2->y -= disp->tam_y * p2->jump_height;
		if(p2->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p2->jump_height = VELOCIDADE_MAX_Y;
			p2->y = disp->tam_y - (p2->height/2 + disp->chao);
			p2->jump = false;
			p2->stamina -= 10;
		}

		//atualiza o valor do salto para efeito da gravidade
		p2->jump_height -= 0.01;
	} else if(keys[84] && p2->attack == 0  && !p2->recuo && p2->y == disp->tam_y - (p2->height/2 + disp->chao) && p2->stamina >= 10)//p2 em posicao valida para saltar
		p2->jump = true;
}

//atualiza a posicao x e y do player1 a partir de seus controles
void move_player_single(player *p1, display_info  *disp, bool *keys){

	int half_side_p1 = p1->side/2;

	//divisor da velocidade (1 velocidade normal, 2 velociadade diminuida)
	int crouch_1 = 1;

	//verifica se o player1 esta agachado e atualiza a velociadade
	if(p1->attack == 0 && !p1->jump  && !p1->recuo && keys[ALLEGRO_KEY_S]){
		p1->crouch = true;
		crouch_1 = 2;
	} else{
		p1->crouch = false;
	}

	if(p1->attack == 0 && !p1->recuo){
		//atualiza a posicao x do player1
		p1->x += (VELOCIDADE_X/crouch_1) * (keys[4] - keys[1]);
		//verifica colisao do player1 com a borda da tela nas laterais
		if(p1->x < half_side_p1 || p1->x > disp->tam_x - half_side_p1)
			p1->x = half_side_p1 * keys[1] + (disp->tam_x - half_side_p1) * keys[4];
	}

	//verifica se o p1 esta saltando
	if(p1->jump){
		
		//atualiza posicao vertical de p1
		p1->y -= disp->tam_y * p1->jump_height;

		//verifica se o pulo foi encerrado
		if(p1->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p1->jump_height = VELOCIDADE_MAX_Y;
			p1->y = disp->tam_y - (p1->height/2 + disp->chao);
			p1->jump = false;
			p1->stamina -= 10;
		}

		//atualiza o valor do salto para efeito da gravidade
		p1->jump_height -= 0.01;
	} else if(keys[23] && p1->attack  == 0 && !p1->recuo && p1->y == disp->tam_y - (p1->height/2 + disp->chao) && p1->stamina >= 10)//p1 em posicao valida para saltar
		p1->jump = true;
}

//verifica se o atancdo acerta o ataque 1 (alto) na vitima
void attack_1(player *atacando, player *vitima){

	if((atacando->sprite_atual < atacando->i_sprites[6] - 1) || vitima->recuo)
		return;

	atacando->stamina -= 30/atacando->frames;
	if(atacando->stamina < 0)
		atacando->stamina = 0;
	
	//indica se houve um hit
	bool acerto = false;

	//atacando para esqueda e vitima dentro do alcance e nao agachada
	if(atacando->olha_esquerda && !vitima->crouch && (vitima->x < atacando->x) && (vitima->x + vitima->side/2 > atacando->x - atacando->side/2 - atacando->attack_1[0]))
		acerto = true;
	else if(!atacando->olha_esquerda && !vitima->crouch && (vitima->x > atacando->x) && (vitima->x - vitima->side/2 < atacando->x + atacando->side/2 + atacando->attack_1[0])) //atacando para direita, vitima dentro do alcance e nao agachada
		acerto = true;

	if(acerto){
		vitima->vida -= 10;
		vitima->recuo = true;
		vitima->attack = 0;
		vitima->attack_done = true;

		//interrompe o salto caso haja hit
		if(vitima->jump && vitima->jump_height > 0)
			vitima->jump_height = -vitima->jump_height;
	}
}

//verifica se o atacando acerta o ataque 2 (baixo) na vitima
void attack_2(player *atacando, player *vitima){
	if((atacando->sprite_atual < atacando->i_sprites[7] - 1) || vitima->recuo)
		return;

	atacando->stamina-= 20/atacando->frames;
	if(atacando->stamina < 0)
		atacando->stamina = 0;

	//atacando para esqueda e vitima dentro do alcance e nao nao esta pulando
	if(atacando->olha_esquerda && (!vitima->jump || abs(vitima->jump_height) < 0.2) && (vitima->x < atacando->x) && (vitima->x + vitima->side/2 > atacando->x - atacando->side/2 - atacando->attack_2[0])){
		vitima->vida -= 5;
		vitima->recuo = true;
		vitima->attack = 0;
		vitima->attack_done = true;
	} else if(!atacando->olha_esquerda && (!vitima->jump && abs(vitima->jump_height) < 0.2) && (vitima->x > atacando->x) && (vitima->x - vitima->side/2 < atacando->x + atacando->side/2 + atacando->attack_2[0])){ //atacando para direita, vitima dentro do alcance e nao esta pulando
		vitima->vida -= 5;
		vitima->recuo = true;
		vitima->attack = 0;
		vitima->attack_done = true;
	} 
}

//verifica o estado atual do player e retorna um subbitmap com a sprite atual que deve ser desenhada
void seleciona_sprite(player *p, int player, bool keys[]){
	int num;
	if(p->recuo){ //player atinjido por ataque
		num = 7;
	} else if(p->jump){//player pulando
		if(p->jump_height >= 0){ //subida do pulo
			num = 2;
		} else {//queda do pulo
			num = 3;
		}
	} else if (p->attack == 0){ //idle, crouch ou andando
		if(p->crouch)//crouch
			num = 4;
		else{
			if(player == 1 && (keys[ALLEGRO_KEY_A] || keys[ALLEGRO_KEY_D]))//p1 andando
				num = 1;
			else if (player == 2 && (keys[ALLEGRO_KEY_LEFT] || keys[ALLEGRO_KEY_RIGHT]))//p2 andando
				num = 1;
			else //idle
				num = 0;
		}
	} else if (p->attack == 1){ //ataque alto
		num = 5;
	} else if (p->attack == 2){ //ataque baixo 
		num = 6;
	}
	bool troca = false;

	//verifica se o estado (jump, idle...) mudou
	if(!(p->sprite_atual >= p->i_sprites[num] && (num == 8 || p->sprite_atual < p->i_sprites[num + 1]))){
		p->sprite_atual = p->i_sprites[num];
		p->tempo_ciclo = 0;
		troca = true;
	}

	//verifica se o ataque acabou
	if(p->attack != 0 && p->sprite_atual + 1 == p->i_sprites[num + 1] && p->tempo_ciclo >= p->frames - 1){
		p->attack_done = true;
		p->attack = 0;
	}

	//verifica se a animacao de hit ja acabou
	if(p->recuo && p->sprite_atual + 1 == p->i_sprites[8] && p->tempo_ciclo >= p->frames - 1)
		p->recuo = false;
	
	//verifica se é necessario mudar a imagem por conta do tempo
	if(p->tempo_ciclo >= p->frames){
		int num_sprites;
		if(num < 8)
			num_sprites = p->i_sprites[num + 1] - p->i_sprites[num];
		else
			num_sprites = p->num_sprites - p->i_sprites[num];
		//troca o indice da imagem (depende de qual animaçao esta ocorrendo no momento)
		p->sprite_atual = p->i_sprites[num] + ((p->sprite_atual - p->i_sprites[num] + 1) % num_sprites);
		troca = true;

		//reinicia o tempo do ciclo atual
		p->tempo_ciclo = 0;
	} else{
		//acrescenta tempo no ciclo atual
		p->tempo_ciclo += 1;
	}

	//cria o subbitmap com a sprite selecionada para impressao
	if(troca){
		al_destroy_bitmap(p->sprite);
		p->sprite = al_create_sub_bitmap(p->bitmap, p->sprite_w * p->sprite_atual, 0, p->sprite_w * (p->sprite_atual + 1), p->sprite_h);
		if(!p->sprite)
			exit(1);
	}
}

//verifica qual a orientacao dos players (esquerda ou direita)
void orientacao_player(player *p1, bool *keys, int player){
	if(player == 1){
		//orientaçao do p1
		if(p1-> attack == 0 && keys[1] && !keys[4])
			p1->olha_esquerda = true;
		else if(p1->attack == 0 && !keys[1] && keys[4])
			p1->olha_esquerda = false;
	} else{
		if(p1->attack == 0 && keys[82] && !keys[83])
			p1->olha_esquerda = true;
		else if(p1->attack == 0 && keys[83] && !keys[82])
			p1->olha_esquerda = false;
	}
}

//reinicia a luta e contapiliza a vitoria de um personagem, caso um deles ganhe 2 wounds enverra a luta e mostra tela de vitoria
void reset_round(player *ganhador, player *perdedor, display_info *disp){

	if(ganhador->vitorias == 0){
		//reinicia a posicao x dos jogadores com base na posicao atual
		if(ganhador->x < perdedor->x){
			ganhador->x = disp->tam_x * 0.1;
			ganhador->olha_esquerda = false;
			perdedor->x = disp->tam_x * 0.9;
			perdedor->olha_esquerda = true;
		} else{
			
			ganhador->x = disp->tam_x * 0.9;
			ganhador->olha_esquerda = true;
			perdedor->x = disp->tam_x * 0.1;
			perdedor->olha_esquerda = false;
		}
	}
	//reinicia a pos_y dos jogadores
	ganhador->y = disp->tam_y  - disp->chao - ganhador->height/2;
	perdedor->y = disp->tam_y  - disp->chao - perdedor->height/2;

	//reinicia a vida dos jogadores
	ganhador->vida = 100;
	perdedor->vida = 100;

	//reinicia a stamina dos jogadores
	ganhador->stamina = 100;
	perdedor->stamina = 100;

	//reinicia os estados de ambos os jogadores
	ganhador->attack = 0;
	perdedor->attack = 0;
	ganhador->attack_done = true;
	perdedor->attack_done = true;
	ganhador->recuo = false;
	perdedor->recuo = false;
	ganhador->jump_height = VELOCIDADE_MAX_Y;
	perdedor->jump_height = VELOCIDADE_MAX_Y;
	ganhador->jump = false;
	perdedor->jump = false;
	ganhador->crouch = false;
	perdedor->crouch = false;


	//atualiza o numero de vitorias go ganhador
	ganhador->vitorias += 1;
}

//destroi um player e seus componentes
player *destroy_player(player *elem){
	if(!elem)
		return elem;
	al_destroy_bitmap(elem->sprite);
	al_destroy_bitmap(elem->bitmap);
	free(elem->attack_1);
	free(elem->attack_2);
	free(elem->i_sprites);
	free(elem);
	return NULL;
}
