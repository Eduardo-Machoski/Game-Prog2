#ifndef __PLAYER__
#define __PLAYER__

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>

typedef struct {
	int *i_sprites; //indica o indice inicial de cada grupo de sprits em uma animacao
	int *attack_1; //tamanho do hitbox do ataque_1 (alto)
	int *attack_2; //tamanho do hitbox do ataque_2 (baixo)
	ALLEGRO_BITMAP *bitmap; //bitmap com todas as sprites do player
	ALLEGRO_BITMAP *sprite; //bitmap com a ultima sprite impressa do player
	float jump_height; //quantidade que o player deve pular se jump == true
	int stamina; //stamina atual
	int vida; //vida atual
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
	int vitorias; //indica numero de rounds que esse player ganhou
	bool olha_esquerda; //indica se a sprite esta olhando para a esquerda
	bool jump; //true se o player estiver pulando
	bool crouch; //true se o player estiver agachado
	bool recuo; //indica se o player esta em estado de recuo apos receber um ataque
	bool attack_done; //indica se um ataque ja foi concluido
	char attack; //indica se um ataque esta sendo realizado e qual (0 se nao)
} player;

#include"display.h"

#define VELOCIDADE_X 10 
#define VELOCIDADE_MAX_Y 0.10


//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int x_ini, bool esquerda, char *pasta);

//verifica se o player pode atacar no momento, se sim atualiza o p->attack para indicar isso
void verifica_ataque(player *p1, player *p2, bool *keys, display_info *disp);

//atualiza a posicao x e y dos players a partir dos seus controles
void move_players(player *p1, player *p2, display_info *disp, bool *keys);

//atualiza a posicao x e y do player1 a partir dos seus controles
void move_player_single(player *p1, display_info *disp, bool *keys);

//verifica se o atacando aceta o ataque 1 (alto) na vitima
void attack_1(player *atacando, player *vitima);

//verifica se o atacando acerto o ataque 2 (baixo) na vitima
void attack_2(player *atacando, player *vitima);

//verifica o estado atual do player e atualiza p->sprite com a sprite atual que deve ser desenhada
void seleciona_sprite(player *p, int player, bool keys[]);

//verifica qual a orientacao dos players (esquerda ou direita)
void orientacao_player(player *p1, bool *keys, int player);

//reinicia a luta e contabiliza a vitoria de um personagem, caso um deles ganhe 2 round encerra a luta e mostra isso
void reset_round(player *ganhador, player *perdedor, display_info *disp);

//destroi um player e seus componentes
player *destroy_player(player *elem);

#endif
