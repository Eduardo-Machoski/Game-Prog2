#include"gui.h"

//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam){
	
	//cria o menu e verifica o alocamento de memoria
	menus *aux;
	if(!(aux = malloc(sizeof(menus))))
		return NULL;

	//cria o vetor de codigos e verifica o alocamento de memoria
	int *codes;
	if(!(codes = calloc(0, sizeof(int) * tam))){
		free(aux);
		return NULL;
	}

	//cria o vetor de strings e verifica o alocamento de memoria
	char **strings;
	if(!(strings = (char**)malloc(sizeof(char*) * tam))){
		free(aux);
		free(codes);
		return NULL;
	}

	//inicializa o menu e o retorna
	aux->opcoes = tam;
	aux->codes = codes;
	aux->atual = 0;
	aux->strings = strings;
	return aux;

}

//menu principal do jogo
bool main_menu(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, bool reset, player *p1, player *p2, bool keys[], ALLEGRO_BITMAP *background){
	//cria o menu que sera usado no display
	menus *m;
	if(!(m = cria_menu(2)))
		return false;

	//cria as strings do menu
	char fala1[] = "Start Game";
	char fala2[] = "Exit Game";

	//inicializa as strings do menu
	m->strings[0] = fala1;
	m->strings[1] = fala2;

	//inicializa os codigos
	m->codes[0] = START_GAME;
       	m->codes[1] = EXIT_GAME;
	
	//mostra o menu e realiza a operacao desejada pelo usuario
	bool aux = display_menu(m, disp, queue, timer, p1, p2, keys, background);

	//menu aberto apos uma partida ter sido iniciada
	if(reset)
		selecao_personagem(disp, &p1, &p2, &background, queue, timer);

	//destroi o menu apos o seu uso
	destroy_menu(m);

	return aux;
}

//menu de selecao de persnagens e background
bool selecao_personagem(display_info *disp, player **p1, player **p2, ALLEGRO_BITMAP **background, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer){

	//pausa o timer do jogo
	al_stop_timer(timer);

	//variaveis que controlam qual personagem os players estao selecionando atualmente
	int p1_i = 0;
	int p2_i = 3;

	//variavel que controla o background selecionado
	int back = 0;

	//verifica se a aplicação nao foi fechada
	bool retorno = false;

	*background = al_load_bitmap("Sprites/Background/background_1.png");

	//imprime a tela de selecao de personagens
	imprime_selecao(disp, p1_i, p2_i, back, *background);

	ALLEGRO_EVENT event;
	al_wait_for_event(queue, &event);

	bool encerra = false;

	//inicia o jogo se "ENTER" for pressionado
	while(!encerra){
		al_wait_for_event(queue, &event);

		//atualiza a selecao atual dos players
		if(event.type == 10){
			if(event.keyboard.keycode == ALLEGRO_KEY_A){
				if(p1_i != 0)
					p1_i--;
				else
					p1_i = 3;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_D){
				if(p1_i != 3)
					p1_i++;
				else
					p1_i = 0;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){
				if(p2_i != 0)
					p2_i--;
				else
					p2_i = 3;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
				if(p2_i != 3)
					p2_i++;
				else
					p2_i = 0;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				full_screen(disp, disp->full, queue);
			else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
				encerra = true;
			else if(event.keyboard.keycode == ALLEGRO_KEY_2 && back == 1){
				al_destroy_bitmap(*background);
				*background = al_load_bitmap("Sprites/Background/background_2.png");
				back = 0;
			} else if(event.keyboard.keycode == ALLEGRO_KEY_1 && back == 0){
				al_destroy_bitmap(*background);
				*background = al_load_bitmap("Sprites/Background/background_1.png");
				back = 1;
			}
			imprime_selecao(disp, p1_i, p2_i, back, *background);
		}

		//verifica se a aplicacao foi fechada
		if(event.type == 42){
			retorno = true;
			encerra = true;
		}
	}

	//p1 ja tinha um heroi selecionado
	if(*p1)
		destroy_player(*p1);

	//p2 ja tinha um heroi selecionado
	if(*p2)
		destroy_player(*p2);

	//cria p1 e p2
	*p1 = cria_player(disp, 10, false);
	*p2 = cria_player(disp, 90, true);

	//reinicia o timer do jogo
	al_start_timer(timer);

	return retorno;
}

//pausa o jogo, removendo todos os inputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, player *p1, player *p2, bool keys[], ALLEGRO_BITMAP *background){

	//cria o menu que sera usado no display
	menus *m;
	if(!(m = cria_menu(3)))
		return false;

	//cria as strings do menu
	char fala1[] = "Return Game";
	char fala2[] = "Main Menu";
	char fala3[] = "Exit Game";

	//inicializa as strings do menu
	m->strings[0] = fala1;
	m->strings[1] = fala2;
	m->strings[2] = fala3;

	//inicializa os codigos
	m->codes[0] = RETURN_GAME;
       	m->codes[1] = MAIN_MENU;
	m->codes[2] = EXIT_GAME;

	//mostra o menu e realiza a operacao desejada pelo usuario
	bool aux = display_menu(m, disp, queue, timer, p1, p2, keys, background);

	//destroi o menu apos o seu uso
	destroy_menu(m);

	return aux;
}


//destroi um menu e seus componentes
void destroy_menu(menus *m){
	free(m->strings);
	free(m->codes);
	free(m);
}
