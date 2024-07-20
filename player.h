#ifndef __PLAYER__
#define __PLAYER__

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>

typedef struct {
	int x; //pos x do player
	int y; //pos y do player
	int side; //tamanho do lado do hitbox
	int side_sprite; //tamanho do lado do hitbox da sprite
	int height; //altura do hitbox
	int height_sprite; //altura da hitbox na sprite
	int sprite_atual; //indice da sprite que o player imprimiu por ultimo
	int tempo_ciclo; //quanto tempo esta com a mesma sprite
	int sprite_w; //width da sprite
	int sprite_h; //height da sprite
	int num_sprites; //numero de sprites que o personagem possui
	int frames; //numero de frames para cada sprite
	int *i_sprites; //indica o indice inicial de cada grupo de sprits em uma animacao
	float vida; //vida atual
	float jump_height; //quantidade que o player deve pular se jump == true
	bool olha_esquerda; //indica se a sprite esta olhando para a esquerda
	bool jump; //true se o player estiver pulando
	bool crouch; //true se o player estiver agachado
	bool recuo; //indica se o player esta em estado de recuo apos receber um ataque
	bool attack_done; //indica se um ataque ja foi concluido
	char attack; //indica se um ataque esta sendo realizado e qual (0 se nao)
	ALLEGRO_BITMAP *bitmap; //bitmap com todas as sprites do player
	ALLEGRO_BITMAP *sprite; //bitmap com a ultima sprite impressa do player
} player;

#include"display.h"

#define VELOCIDADE_X 10 
#define VELOCIDADE_MAX_Y 0.09


//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int x_ini, bool esquerda);

//verifica se o player pode atacar no momento, se sim atualiza o p->attack para indicar isso
void verifica_ataque(player *p1, player *p2, bool *keys);

//atualiza a posicao x e y dos players a partir dos seus controles
void move_players(player *p1, player *p2, display_info *disp, bool *keys);

//verifica e corrige colisao entre players
void colisao_players(player *p1, player *p2, bool *keys);

//verifica o estado atual do player e atualiza p->sprite com a sprite atual que deve ser desenhada
void seleciona_sprite(player *p);

//verifica qual a orientacao dos players (esquerda ou direita)
void orientacao_players(player *p1, player *p2, bool *keys);

//destroi um player e seus componentes
void destroy_player(player *elem);

#endif
