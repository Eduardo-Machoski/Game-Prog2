#include"player.h"

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int ini_x){

	//cria a estrutura e verifica a alocação de memoria
	player *aux;
        if(!(aux = malloc(sizeof(player))))
	       exit(1);

	//bitmap para as sprites
	ALLEGRO_BITMAP *bitmap = NULL;
	if(!(bitmap = al_load_bitmap("Sprites/Martial_Hero/Teste1.png")))
		exit(1);

	ALLEGRO_BITMAP *sprite = NULL;
	if(!(sprite = al_create_sub_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap)/4, al_get_bitmap_height(bitmap))))
		exit(1);

	//inicializa o player
	aux->side = disp->tam_x / 16;
	aux->height = disp->tam_x / 8;
	aux->vida = 100;
	aux->jump = false;
	aux->jump_height = VELOCIDADE_MAX_Y;
	aux->bitmap = bitmap;
	aux->sprite = sprite;
	aux->sprite_atual = 0;
	aux->sprite_w = al_get_bitmap_width(bitmap) / 4;
	aux->sprite_h = al_get_bitmap_height(bitmap);

	//posicao horizontal inicial (ini_x% da tela)
	aux->x = disp->tam_x * (ini_x/100.0);

	//inicializa os players no chao)
	aux->y = disp->tam_y - disp->chao - aux->height/2;
	return aux;
}



//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, display_info  *disp, bool *keys){

	int half_side_p1 = p1->side/2;
	int half_side_p2 = p2->side/2;

	//atualiza a posicao x do player1
	p1->x += VELOCIDADE_X * (keys[4] - keys[1]);
	//verifica colisao do player1 com a borda da tela nas laterais
	if(p1->x < half_side_p1 || p1->x > disp->tam_x - half_side_p1)
		p1->x = half_side_p1 * keys[1] + (disp->tam_x - half_side_p1) * keys[4];

	
	//atualiza a posicao x do player2
	p2->x += VELOCIDADE_X * (keys[83] - keys[82]);
	//verifica colisao do player2 com a borda da tela nas lateraiss
	if(p2->x < half_side_p2 || p2->x > disp->tam_x - half_side_p2)
		p2->x = half_side_p2 * keys[82] + (disp->tam_x - half_side_p2) * keys[83];

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
	} else if(keys[23] && p1->y == disp->tam_y - (p1->height/2 + disp->chao)) //p1 em posicao valida para saltar
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
	} else if(keys[84] && p2->y == disp->tam_y - (p2->height/2 + disp->chao)) //p2 em posicao valida para saltar
		p2->jump = true;

	//verifica e arruma colisao entre players
	colisao_players(p1, p2, keys);

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

//verifica o estado atual do player e retorna um subbitmap com a sprite atual que deve ser desenhada
void seleciona_sprite(player *p){
	bool troca = false;
	
	//verifica se é necessario mudar a imagem por conta do tempo
	if(p->tempo_ciclo >= 5){
		int num_sprites = 4;
		//troca o indice da imagem (depende de qual animaçao esta ocorrendo no momento)
		p->sprite_atual = (p->sprite_atual + 1) % num_sprites;
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

//destroi um player e seus componentes
void destroy_player(player *elem){
	al_destroy_bitmap(elem->bitmap);
	free(elem);
}
