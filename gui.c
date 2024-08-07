#include"gui.h"

//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam){
	
	//cria o menu e verifica o alocamento de memoria
	menus *aux;
	if(!(aux = malloc(sizeof(menus))))
		return NULL;

	//cria o vetor de codigos e verifica o alocamento de memoria
	int *codes;
	if(!(codes = malloc(sizeof(int) * tam))){
		free(aux);
		return NULL;
	}

	for(int i = 0; i < tam; i++)
		codes[i] = 0;

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
bool main_menu(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, bool reset, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single){
	//cria o menu que sera usado no display
	menus *m;
	if(!(m = cria_menu(3)))
		return false;

	//cria as strings do menu
	char fala0[] = "skull.png";
	char fala1[] = "new_game.png";
	char fala2[] = "quit.png";

	//inicializa as strings do menu
	m->strings[0] = fala0;
	m->strings[1] = fala1;
	m->strings[2] = fala2;

	//inicializa os codigos
	m->codes[0] = BOSS;
	m->codes[1] = START_GAME;
       	m->codes[2] = EXIT_GAME;
	
	//mostra o menu e realiza a operacao desejada pelo usuario
	bool aux = display_menu(m, disp, queue, timer, p1, p2, keys, NULL, single);

	//menu aberto apos uma partida ter sido iniciada
	if(reset && !single)
		selecao_personagem(disp, p1, p2, &background, queue, timer);

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

	//players com personagens ja selecionados	
	*p1 = destroy_player(*p1);
	*p2 = destroy_player(*p2);	

	//indica a pasta do personagem selecionado pelo player 1
	char *pasta_1 = malloc(sizeof(char) * 1000);
	if(!pasta_1)
		exit(1);
	pasta_1[0] = '\0';

	//indica a pasta do personagem selecionado pelo player 2
	char *pasta_2 = malloc(sizeof(char) * 1000);
	if(!pasta_2)
		exit(1);
	pasta_2[0] = '\0';

	//indica a pasta de sprites
	strcat(pasta_1, "Sprites/");
	strcat(pasta_2, "Sprites/");

	//indica a pasta correta do player 1
	if(p1_i == 0)
		strcat(pasta_1, "Fantasy_Warrior/");
	else if(p1_i == 1)
		strcat(pasta_1, "Martial_Hero/");
	else if(p1_i == 2)
		strcat(pasta_1, "Martial_Hero_3/");
	else
		strcat(pasta_1, "Medieval_Warrior/");

	//indica a pasta correta do player 2
	if(p2_i == 0)
		strcat(pasta_2, "Fantasy_Warrior/");
	else if(p2_i == 1)
		strcat(pasta_2, "Martial_Hero/");
	else if(p2_i == 2)
		strcat(pasta_2, "Martial_Hero_3/");
	else
		strcat(pasta_2, "Medieval_Warrior/");

	//cria p1 e p2
	*p1 = cria_player(disp, 10, false, pasta_1);
	*p2 = cria_player(disp, 90, true, pasta_2);

	//destroi a string que indicava a pasta dos players
	free(pasta_1);
	free(pasta_2);

	//reinicia o timer do jogo
	al_start_timer(timer);

	return retorno;
}

bool selecao_single(display_info *disp, player **p1, ALLEGRO_BITMAP **background, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer){
	//pausa o timer do jogo
	al_stop_timer(timer);

	//variaveis que controlam qual personagem os players estao selecionando atualmente
	int p1_i = 0;

	//variavel que controla o background selecionado
	int back = 0;

	//verifica se a aplicação nao foi fechada
	bool retorno = false;

	*background = al_load_bitmap("Sprites/Background/background_1.png");

	//imprime a tela de selecao de personagens
	imprime_selecao_single(disp, p1_i, back, *background);

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
			imprime_selecao_single(disp, p1_i, back, *background);
		}

		//verifica se a aplicacao foi fechada
		if(event.type == 42){
			retorno = true;
			encerra = true;
		}
	}

	//players com personagens ja selecionados	
	*p1 = destroy_player(*p1);

	//indica a pasta do personagem selecionado pelo player 1
	char *pasta_1 = malloc(sizeof(char) * 1000);
	pasta_1[0] = '\0';

	//indica a pasta de sprites
	strcat(pasta_1, "Sprites/");

	//indica a pasta correta do player 1
	if(p1_i == 0)
		strcat(pasta_1, "Fantasy_Warrior/");
	else if(p1_i == 1)
		strcat(pasta_1, "Martial_Hero/");
	else if(p1_i == 2)
		strcat(pasta_1, "Martial_Hero_3/");
	else
		strcat(pasta_1, "Medieval_Warrior/");

	//cria p1 e p2
	*p1 = cria_player(disp, 10, false, pasta_1);

	//destroi a string que indicava a pasta dos players
	free(pasta_1);

	//reinicia o timer do jogo
	al_start_timer(timer);

	return retorno;
}

//pausa o jogo, removendo todos os inputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, display_info *disp, ALLEGRO_TIMER *timer, player **p1, player **p2, bool keys[], ALLEGRO_BITMAP *background, bool *single){

	//cria o menu que sera usado no display
	menus *m;
	if(!(m = cria_menu(3)))
		return false;

	//cria as strings do menu
	char fala1[] = "resume.png";
	char fala2[] = "menu.png";
	char fala3[] = "quit.png";

	//inicializa as strings do menu
	m->strings[0] = fala1;
	m->strings[1] = fala2;
	m->strings[2] = fala3;

	//inicializa os codigos
	m->codes[0] = RETURN_GAME;
       	m->codes[1] = MAIN_MENU;
	m->codes[2] = EXIT_GAME;

	//mostra o menu e realiza a operacao desejada pelo usuario
	bool aux = display_menu(m, disp, queue, timer, p1, p2, keys, background, single);

	//destroi o menu apos o seu uso
	destroy_menu(m);

	return aux;
}

//Diz quem foi o ganhador, realiza a animacao de morte do perdedor e verifica se quer hogar outra, retornar ao menu
//ou sair do jogo
bool tela_vitoria(player **p1, player **p2, display_info *disp, int num, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP **background, bool keys[], ALLEGRO_FONT *font, bool *single){


	//realiza a animacao de morte do player derrotado e idle do outro.
	//imprime o texto indicando qual o ganhador
	if(num == 2)	
		animacao_morte(*p1, *p2, disp, font, queue, *background, 50);
	else
		animacao_morte(*p2, *p1, disp, font, queue, *background, 49);

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
	bool aux = display_menu(m, disp, queue, timer, p1, p2, keys, *background, single);

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
