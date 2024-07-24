#include"player.h"

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int ini_x, bool esquerda){

	//cria a estrutura e verifica a alocação de memoria
	player *aux;
        if(!(aux = malloc(sizeof(player))))
	       exit(1);

	//bitmap para as sprites
	ALLEGRO_BITMAP *bitmap = NULL;
	if(!(bitmap = al_load_bitmap("Sprites/Fantasy_Warrior/sprite.png")))
		exit(1);

	//abre a file com informações das sprites
	FILE *file;
	const char *name = "./Sprites/Fantasy_Warrior/info.txt";
	if(!(file = fopen(name, "r")))
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
	if(!(indice_sprites = malloc(sizeof(int) * 7)))
		exit(1);
	for(int i = 0; i < 7; i++)
		fscanf(file, "%d", &indice_sprites[i]);

	int frames;
	fscanf(file, "%d", &frames);

	//tamanhos do hitbox de cada ataque
	int *attack_1, *attack_2;
	if(!(attack_1 = malloc(sizeof(int) * 2)))
		exit(1);

	if(!(attack_2 = malloc(sizeof(int) * 2)))
		exit(1);

	fscanf(file, "%d %d", &attack_1[0], &attack_1[1]);
	fscanf(file, "%d %d", &attack_2[0], &attack_2[1]);

	attack_1[0] = (disp->tam_x/8) * (attack_1[0]/(float)width);
	attack_1[1] = (disp->tam_x/8) * (attack_1[1]/(float)width);


	//inicializa o player
	aux->side = disp->tam_x / 16;
	aux->side_sprite = (disp->tam_x / 16) * (width/tam_x);	
	aux->height = disp->tam_y / 4;
	aux->height_sprite = (disp->tam_y / 4) * (height/tam_y);
	aux->vida = 1;
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

	//posicao horizontal inicial (ini_x% da tela)
	aux->x = disp->tam_x * (ini_x/100.0);

	//inicializa os players no chao)
	aux->y = disp->tam_y - disp->chao - aux->height/2;

	//fecha a file de informacao
	fclose(file);
	return aux;
}

//verifica se o player pode atacar no momento, se sim atualiza o p->attack para indicar isso
void verifica_ataque(player *p1, player *p2, bool *keys){
	if(!p1->jump && !p1->crouch && p1->attack == 0){ //verifica se o p1 pode atacar
		if(keys[26]){//ataque baixo
			p1->attack = 1;
			p1->attack_done = false;
		}
		if(keys[24]){//ataque alto
			p1->attack = 2;
			p1->attack_done = false;
		}
	}

	if(!p2->jump && !p2->crouch && p2->attack == 0){//verifica se o p2 pode atacar
		if(keys[73]){//ataque baixo
			p2->attack = 1;
			p2->attack_done = false;
		}
		if(keys[68]){//ataque alto
			p2->attack = 2;
			p2->attack_done = false;
		}
	}

	//verifica hits dos ataques
	attack_1(p1, p2);
	attack_1(p2, p1);
}

//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, display_info  *disp, bool *keys){

	int half_side_p1 = p1->side/2;
	int half_side_p2 = p2->side/2;
	
	if(p1->attack == 0){
		//atualiza a posicao x do player1
		p1->x += VELOCIDADE_X * (keys[4] - keys[1]);
		//verifica colisao do player1 com a borda da tela nas laterais
		if(p1->x < half_side_p1 || p1->x > disp->tam_x - half_side_p1)
			p1->x = half_side_p1 * keys[1] + (disp->tam_x - half_side_p1) * keys[4];
	}

	if(p2->attack == 0){	
		//atualiza a posicao x do player2
		p2->x += VELOCIDADE_X * (keys[83] - keys[82]);
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
		}

		//atualiza o valor do salto para efeito da gravidade
		p1->jump_height -= 0.01;
	} else if(keys[23] && p1->attack  == 0 && !p1->recuo && p1->y == disp->tam_y - (p1->height/2 + disp->chao)) //p1 em posicao valida para saltar
		p1->jump = true;

	//verifica se o p2 esta saltando
	if(p2->jump){

		//atualiza posicao vertical de p2
		p2->y -= disp->tam_y * p2->jump_height;
		if(p2->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p2->jump_height = VELOCIDADE_MAX_Y;
			p2->y = disp->tam_y - (p2->height/2 + disp->chao);
			p2->jump = false;
		}

		//atualiza o valor do salto para efeito da gravidade
		p2->jump_height -= 0.01;
	} else if(keys[84] && p2->attack == 0  && !p2->recuo && p2->y == disp->tam_y - (p2->height/2 + disp->chao)) //p2 em posicao valida para saltar
		p2->jump = true;

	//verifica e arruma colisao entre players
	//colisao_players(p1, p2, keys);

	return;
}

//verifica e arruma colisao entre players
void colisao_players(player *p1, player *p2, bool *keys){
	//verifica se osplayers estao na mesma altura relativa
	if(p1->y > p2->y + p2->height * 0.45 || p2->y > p1->y + p1->height * 0.45)
		return;

	//verifica qual player está na esquerda e qual na direita em relacao ao outro
	player *esq, *dir;
	int esq_half, dir_half;

	//guarda os movimentos dos players
	bool esq_move_dir, dir_move_esq;
	bool esq_move_esq, dir_move_dir;
	if(p1->x < p2->x){
		esq = p1;
		dir = p2;
		esq_half = p1->side / 2;
		dir_half = p2->side / 2;
		esq_move_dir = keys[4];
		dir_move_esq = keys[82];
		esq_move_esq = keys[1];
		dir_move_dir = keys[83];
	} else {
		esq = p2;
		dir = p1;
		esq_half = p2->side / 2;
		dir_half = p1->side / 2;
		esq_move_dir = keys[83];
		dir_move_esq = keys[1];
		esq_move_esq = keys[82];
		dir_move_dir = keys[4];
	}

	//sem colisao entre players
	if(esq->x + esq_half <= dir->x - dir_half)
		return;

	//verifica se os players estao andando
	if(!(dir_move_esq || esq_move_dir || esq_move_esq || dir_move_dir)){
		if(dir->y < esq->y)
			dir->x = esq->x + dir_half + esq_half;
		else
			esq->x = dir->x - (dir_half + esq_half);
	} else if(!dir_move_dir && !esq_move_esq){//ambos os players sem input para o lado oposto da colisao
		//player direito andando para esquerda e player esquerdo parado
		if(!esq_move_dir){
			dir->x = esq->x + dir_half + esq_half;
		} else if(!dir_move_esq){//player esquerdo andando para direita e player direito parado
			esq->x = dir->x - (dir_half + esq_half);
		} else{ //ambos parados ou se movimentandoem em sentido de colisao
			int collision = esq->x + (dir->x - esq->x)/2;
			esq->x = collision - esq_half;
			dir->x = collision + dir_half;
		}
	} else if(!dir_move_dir){//player esquerdo se afastando do direito
		if(esq_move_dir) //player esquerdo parado
			dir->x = esq->x + dir_half + esq_half;
		else if(dir_move_esq){//ambos se movimentdando na mesma direcao
			int collision = esq->x + (dir->x - esq->x)/2;
			esq->x = collision - esq_half;
			dir->x = collision + dir_half;
		} else{//player direito parado
			esq->x = dir->x - (dir_half + esq_half);
		}
	} else{//player direito se afalstando do esquerdo
		if(dir_move_esq) //player direito parado
			esq->x = dir->x - (esq_half + dir_half);
		else if(esq_move_dir){//ambos se movimentando na mesma direcao
			int collision = esq->x + (dir->x - esq->x)/2;
			esq->x = collision - esq_half;
			dir->x = collision + dir_half;
		} else{//player esquerdo parado
			dir->x = esq->x + (dir_half + esq_half);
		}
	}
}

//caso atacando esteja realizando o attack_1 verifica se houve hit na vitima
void attack_1(player *atacando, player *vitima){
	//player nao esta atacando
	if(atacando->attack != 1)
		return;

	if((atacando->sprite_atual >= atacando->i_sprites[4] - 2) && atacando->olha_esquerda)
		al_draw_rectangle(atacando->x - atacando->side/2 - atacando->attack_1[0], atacando->y - atacando->height/2, atacando->x - atacando->side/2, atacando->y + atacando->attack_1[1], al_map_rgb(255,0,0), 1);

	if((atacando->sprite_atual >= atacando->i_sprites[4] - 2) && !atacando->olha_esquerda)
		al_draw_rectangle(atacando->x + atacando->side/2 + atacando->attack_1[0], atacando->y - atacando->height/2, atacando->x + atacando->side/2, atacando->y + atacando->attack_1[1], al_map_rgb(255,0,0), 1);
}

//verifica o estado atual do player e retorna um subbitmap com a sprite atual que deve ser desenhada
void seleciona_sprite(player *p){
	int num;
	if(p->jump){
		if(p->jump_height >= 0){ //subida do pulo
			num = 1;
		} else {//queda do pulo
			num = 2;
		}
	} else if (p->attack == 0){ //idle
		num = 0;
	} else if (p->attack == 1){ //ataque baixo
		num = 3;
	} else if (p->attack == 2){ //ataque alto
		num = 4;
	}
	bool troca = false;

	//verifica se o estado (jump, idle...) mudou
	if(!(p->sprite_atual >= p->i_sprites[num] && (num == 6 || p->sprite_atual < p->i_sprites[num + 1]))){
		p->sprite_atual = p->i_sprites[num];
		p->tempo_ciclo = 0;
		troca = true;
	}

	//verifica se o ataque acabou
	if(p->attack != 0 && p->sprite_atual +1 == p->i_sprites[num + 1] && p->tempo_ciclo >= p->frames - 1){
		p->attack_done = true;
		p->attack = 0;
	}
	
	//verifica se é necessario mudar a imagem por conta do tempo
	if(p->tempo_ciclo >= p->frames){
		int num_sprites;
		if(num < 6)
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
void orientacao_players(player *p1, player *p2, bool *keys){
	//orientaçao do p1
	if(p1-> attack == 0 && keys[1] && !keys[4])
		p1->olha_esquerda = true;
	else if(p1->attack == 0 && !keys[1] && keys[4])
		p1->olha_esquerda = false;

	//orientacao do p2
	if(p2->attack == 0 && keys[82] && !keys[83])
		p2->olha_esquerda = true;
	else if(p2->attack == 0 && keys[83] && !keys[82])
		p2->olha_esquerda = false;
}

//destroi um player e seus componentes
void destroy_player(player *elem){
	al_destroy_bitmap(elem->bitmap);
	al_destroy_bitmap(elem->sprite);
	free(elem->attack_1);
	free(elem->attack_2);
	free(elem->i_sprites);
	free(elem);
}
