#include "main.h"

#define SCREEN_WIDTH		256
#define SCREEN_HEIGHT		200

using namespace std;

int main ( int argc, char* argv[] ) 
{
	bool running = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "Incapaz de inicializar o SDL: %s\n", SDL_GetError() );
		running = false;
	}
	atexit(SDL_Quit);

	/* Cria a screen */
	SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16,
		SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_WM_SetCaption( "Open Paradise Cafe", "Open Paradise Cafe" );

	if ( !screen ) 
	{
		printf("Incapaz de inicializar a %ix%i: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError() );
		running = false;
	}

	SDL_Event keyevent;
	
	/* Cria-se o Heroi e define-se a cor da transparencia */
	Sprite Hero("sprites/sprites.bmp",4,300);
	Hero.setTransparency(255,0,255);

	/* Cria-se a Porta */
	Sprite Porta("sprites/porta.bmp",5,300);

	/* Parede */
	SDL_Rect Wall;
	Wall.x = 0;
	Wall.y = 0;
	Wall.w = SCREEN_WIDTH;
	/*Wall.h = 136;*/
	Wall.h = SCREEN_HEIGHT*0.68;

	/* Chão */
	SDL_Rect Floor;
	Floor.x = 0;
	/*Floor.y = 136;*/
	Floor.y = SCREEN_HEIGHT*0.68;
	Floor.w = SCREEN_WIDTH;
	Floor.h = 24;

	/* Linha do HiScore */
	SDL_Rect HiScore;
	HiScore.x = 0;
	HiScore.y = 160;
	HiScore.w = SCREEN_WIDTH;
	HiScore.h = 8;


	/* Status */
	SDL_Rect Status;
	Status.x = 0;
	/*Status.y = 262 + 24;*/
	Status.y = 168;
	Status.w = SCREEN_WIDTH;
	/*Status.h = SCREEN_HEIGHT - 262 - 24;*/
	Status.h = 40;

	/* Posição em X da Porta */
	int PortaX = 350;

	while(running) 
	{
		/* Parede */
		SDL_FillRect(screen, &Wall, SDL_MapRGB(screen->format, 0xc8, 0x41, 0x34));

		/* Chão */
		SDL_FillRect(screen, &Floor, SDL_MapRGB(screen->format, 0xc6, 0x0, 0xc6));

		/* HiScore */
		SDL_FillRect(screen, &HiScore, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));

		/* Status */
		SDL_FillRect(screen, &Status, SDL_MapRGB(screen->format, 0x0, 0x0, 0xc6));

		/* Desenha a Porta */
		Porta.draw(screen,PortaX,0);

		/* Desenha o Heroi */
		/*Hero.draw(screen,150,150);*/
		Hero.draw(screen,50,24);

		/* Se chegar ao fim da linha - vai po inicio */
		if (PortaX < 0)
		{
			PortaX = SCREEN_WIDTH;
		}

		/* Se a porta colidir com o Heroi */
		if ( Porta.pixelCollide(PortaX,50,Hero,50,24) )
		{
			/* Faz a animação da porta ate ao fim */
			if (Porta.getFrame() < 4) 
			{
				Porta.animate();
			}
		}
		else
		{
			/* Animação do Heroi e movimento da Porta */
			Hero.animate();
			PortaX -= 10;
		}

		SDL_Flip(screen);

		/* Aguarda um momento */
		SDL_Delay(200);

		SDL_PollEvent(&keyevent);
		
		switch(keyevent.type) 
		{
			case SDL_KEYDOWN:
				printf("key down\n");
				switch(keyevent.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						running = false;
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
		}

	}

	return 0;
}
