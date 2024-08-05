#ifndef __SINGLE__
#define __SINGLE__


#include<allegro5/allegro.h>
#include<allegro5/allegro_ttf.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct{
	int *i_sprites; //indica o indice inicial de cada grupo de sprits em uma animacao
        int *sprite_atual; //vetor dos indices das sprites que o boss imprimiu por ultimo
        int *tempo_ciclo; //vetor do tempo de cada sprite
        int *num_sprites; //vetor do numero de sprites que o personagem possui
        ALLEGRO_BITMAP **bitmap; //vetor de bitmaps com todas as sprites do boss
        ALLEGRO_BITMAP **sprite; //vetor de bitmap com as ultimas sprites impressa do boss
        int vida; //vida atual
        int x; //pos x do boss
        int y; //pos y do boss
        int side; //tamanho do lado do hitbox
        int side_sprite; //tamanho do lado do hitbox da sprite
        int height; //altura do hitbox
        int height_sprite; //altura da hitbox na sprite
        int sprite_w; //width da sprite
        int sprite_h; //height da sprite
        bool olha_esquerda; //indica se a sprite esta olhando para a esquerda
        int attack; //indica se um ataque esta sendo realizado e qual (0 se nao)
	bool recuo; //indica se o boss foi acertado pelo player
	int attack_resta; //numero de ataques faltantes ate voltar ao attack 0 (idle)

} boss;


typedef struct{
	int *pos_x; //posicao x de cada ataque
	int *pos_y; //posicao y de cada ataque
	bool *low; //informa se o ataque pode ser pulado
	bool *high; //informa se o ataque pode ser desviado abaixando
	bool *valido; //informa se um ataque ainda é valido
	int num; //numero de ataques
	int code; //codigo do tipo de ataque
	int tam_x; //tamanho do hitbox em x do ataque
	int tam_y; //tamanho do hitbox em y do ataque
} ataque;


#include"player.h"
#include"display.h"
#include"gui.h"

#define SPRITES_BOSS 3
#define BOLA 1
#define COLUNA 2

//main do modo single player
bool single_player(display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP **background, ALLEGRO_FONT *font, bool keys[], player **p, bool *single, int dev_mode);

//movimenta o boss (tanto as animações quanto o estado de ataque)
void movimento_boss(boss *b, int *ciclo, display_info *disp);

//verifica o status do boss e retorna qual a animacao ele deve realizar no momento
int status_boss(boss *b);

//verifica se o player realiza um ataque, se sim verifica se ha hit no boss
//verifica se o ataque ja acabou
void ataque_player(player *p, bool keys[], bool hitbox, boss *b);

//verifica se o boss realiza um ataque, se sim verifica se ha hit no player
//verifica se o ataque ja acabou
void ataques_boss(player *p, bool keys[], bool hitbox, boss *b, ataque *attack, display_info *disp);

//verifica se o player ganhou ou perdeu o modo single player
//realiza o encerramento dependendo se ganhou ou perdeu
//reinicia o boss e o player se new game
//sai do modo single player se main menu ou exit
bool verifica_fim(player *p, boss *b, display_info *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP *background, bool keys[], ALLEGRO_FONT *font, bool *retorno, bool *single);

//adiciona um ataque na struct ataque
void adiciona_ataque(ataque *attack, int x, int y, int tam_x, int tam_y, bool low, bool high, int code);

//cria a struct que controla os ataques do boss
ataque *cria_ataque();

//'anda' a bola de fogo em direcao a esquerda e verifica hit no player
void andar_ataque(ataque *attack, player *p);

//verifica se o player foi atinjigo pelas colunas de fogo
bool verifica_hit(ataque *attack, player *p, boss *b);

//destroi a struct ataque e todos os seus componentes
void destroy_ataque(ataque *elem);

//cria a struct do boss e inicializa ela
boss *cria_boss(display_info *disp);

//destroi a struct do boss e todos os seus componentes
void destroy_boss(boss *elem);

#endif
