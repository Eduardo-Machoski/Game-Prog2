#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>

#include"player.h"
#include"gui.h"
#include"display.h"

int main(){
	//inicializa os componentos do allegro
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();

	//cria os componentes do allegro
	ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();

	//cria o display e armazena suas informacoes
	display_info *disp = cria_display();

	//regista os eventos que serao colocados na queue
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp->disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	//começa o relogio do programa
	al_start_timer(timer);


	//cria amobos os players
	player *player_1 = cria_player(disp, 10);
	player *player_2 = cria_player(disp, 90);

	//evento atual sendo lidado no loop
	ALLEGRO_EVENT event;

	//codigo do evento atual
	int code;

	//controle de movimento dos personagens
	bool pressed_keys[ALLEGRO_KEY_MAX] = {false};

	//se true encerra o programa
	bool encerra = false;

	//roda até que o programa seja encerrado
	while(!encerra){
		//aguarda e obtem o proximo evento, assim como seu codigo
		al_wait_for_event(queue, &event);
		code = event.type;
		
		//tick do timer
		if(code == 30){
			move_players(player_1, player_2, disp, pressed_keys);
			al_clear_to_color(al_map_rgb(0,0,0));
			imprime_players(player_1, player_2);
			al_flip_display();
		} else if(code == 10 || code == 12){ //tecla pressionada
			pressed_keys[event.keyboard.keycode] = pressed_keys[event.keyboard.keycode] ^ 1;

			//printf("%d", event.keyboard.keycode);

			//pressiona 'f11' para obter tela full-screen
			if(event.keyboard.keycode == 57 && code == 10){
				al_unregister_event_source(queue, al_get_display_event_source(disp->disp));
				if(disp->full)
					full_screen(disp, true);
				else
					full_screen(disp, false);
				al_register_event_source(queue, al_get_display_event_source(disp->disp));
			}

		} else if(code == 42) //botao de fechar pressionado
			encerra = true;
	}

	//destroi os jogadores e seus componentes
	destroy_player(player_1);
	destroy_player(player_2);

	//destroi o display e sua estrutura auxiliar
	destroy_display_info(disp);

	//destroi os componentes do allegro
	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_uninstall_keyboard();
	al_uninstall_system();

	return 0;
}
