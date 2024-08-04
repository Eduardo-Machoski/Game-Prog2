#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include<allegro5/allegro.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>

#include"player.h"
#include"gui.h"
#include"display.h"

int main(int argc, char *argv[]){
	bool dev_mode = false;

	if(argc == 2 && strcmp(argv[1], "dev") == 0)
		dev_mode = true;

	//inicializa os componentos do allegro
	al_init();
	al_init_primitives_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();

	//cria os componentes do allegro
	ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_load_ttf_font("Sprites/Font/font.ttf", 40, ALLEGRO_TTF_MONOCHROME);

	//cria o display e armazena suas informacoes
	display_info *disp = cria_display();

	//regista os eventos que serao colocados na queue
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp->disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	//controle de movimento dos personagens
	bool pressed_keys[ALLEGRO_KEY_MAX] = {false};
	
	//background a ser selecionado na selecao de personagens
	ALLEGRO_BITMAP *background = NULL;

	//abre o menu principal no inicio do jogo
	//caso a variavel encerra == true, encerra o programa
	bool encerra = main_menu(queue, disp, timer, false, NULL, NULL, pressed_keys, background);

	//evento atual sendo lidado no loop
	ALLEGRO_EVENT event;

	//codigo do evento atual
	int code;

	if(!(background = al_load_bitmap("Sprites/Background/background_2.png")))
		exit(1);

	player *player_1 = NULL, *player_2 = NULL;

	//seleçao de personagem caso "Start Game" seja selecionado
	//seleciona background tambem
	if(!encerra)
		encerra = selecao_personagem(disp, &player_1, &player_2, &background, queue, timer);

	al_start_timer(timer);

	//roda até que o programa seja encerrado
	while(!encerra){
		//aguarda e obtem o proximo evento, assim como seu codigo
		al_wait_for_event(queue, &event);
		code = event.type;
		
		//tick do timer
		if(code == 30){
			verifica_ataque(player_1, player_2, pressed_keys, disp);
			if(player_1->vitorias == 2)
				encerra = tela_vitoria(&player_1, &player_2, disp, 1, timer, queue, &background, pressed_keys);
			else if (player_2->vitorias == 2)
				encerra = tela_vitoria(&player_1, &player_2, disp, 2, timer, queue, &background, pressed_keys);
			move_players(player_1, player_2, disp, pressed_keys);
			imprime_background(background, disp);
			imprime_vida(disp, player_1, player_2);
			imprime_score(player_1->vitorias, player_2->vitorias, disp, font);
			imprime_players(player_1, player_2, pressed_keys, dev_mode, false);
			al_flip_display();
		} else if(code == 10){ //tecla pressionada
			//atualiza o controle de movimento dos personagens
			pressed_keys[event.keyboard.keycode] = 1;

			//pressiona 'f11' para obter tela full-screen
			if(event.keyboard.keycode == 57){
				al_unregister_event_source(queue, al_get_display_event_source(disp->disp));
				if(disp->full)
					full_screen(disp, true, queue);
				else
					full_screen(disp, false, queue);
				al_register_event_source(queue, al_get_display_event_source(disp->disp));
			}

			//pressiona 'esc' para pausar o jogo e abrir o menu de pausa
			if(event.keyboard.keycode == 59)
				encerra = pause_gui(queue, disp, timer, &player_1, &player_2, pressed_keys, background);

		} else if(code == 12){ // tecla liberada
			//atualiza o controle de movimento dos personagens
			pressed_keys[event.keyboard.keycode] = 0;
		} else if(code == 42) //botao de fechar pressionado
			encerra = true;
	}

	//destroi os jogadores e seus componentes
	destroy_player(player_1);
	destroy_player(player_2);

	//destroi o display e sua estrutura auxiliar
	destroy_display_info(disp);

	//destroi os componentes do allegro
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_uninstall_keyboard();
	al_uninstall_system();

	return 0;
}
