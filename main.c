/* this game can never fail */
#include "SDL.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SPRITE_WIDTH     40
#define SPRITE_HEIGHT     128

#define DOOR_SIZE 400

#define BACK_SIZE	32

int gameover;

/* source and destination rectangles */
SDL_Rect rcSrc, rcSprite;
SDL_Rect rcSrcDoor, rcDoor;

int i;

void HandleEvent(SDL_Event event)
{
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;
		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q: /* bye bye */
					gameover = 1;
					break;
				case SDLK_LEFT:
					rcSrc.x = (rcSrc.x + 40)%160; /* sprite rectangle movement */
					rcDoor.x -= 10; /* door movement */
					break;
				case SDLK_RIGHT:
					rcSrc.x = (rcSrc.x + 40)%160;
					rcDoor.x += 10;
					break;
			}
			break;
		case SDL_USEREVENT:
			switch (event.user.code) {
				case 1:
					if (rcSrcDoor.x == 0) 
					for (i = 0; i <= 4; i++) {
						rcSrcDoor.x = i*56;
						rcSrcDoor.y = 0;
						rcSrcDoor.w = 56;
						rcSrcDoor.h = 136;
					}
			}
			break;
				
	}
}

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *temp, *sprite, *door, *background, *floor; 
	SDL_Rect rcBackground; /* background rectangle */
	SDL_Rect rcFloor; /* chaozinho roxo */
	int colorkey;
	int x,y;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("Open Paradise Cafe", "Open Paradise Cafe");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(70, 70);

	/* load sprite damatta */
	temp = SDL_LoadBMP("sprites.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load background image  */
	temp  = SDL_LoadBMP("back.bmp");
	background = SDL_DisplayFormat(temp);

	/* ler o chao */
	temp  = SDL_LoadBMP("ground.bmp");
	floor = SDL_DisplayFormat(temp);

	/* ler as portas */
	temp  = SDL_LoadBMP("porta.bmp");
	door = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup sprite colorkey and turn on RLE */
	colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	SDL_SetColorKey(door, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* posicao inicial do damatta */
	rcSprite.x = 150;
	rcSprite.y = 150;

	/* define o tamanho e o *frame* do boneco */
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	/* define o tamanho das portas */
	rcSrcDoor.x = 0;
	rcSrcDoor.y = 0;
	rcSrcDoor.w = 56; // so a primeira frame
	rcSrcDoor.h = door->h;

	rcDoor.x = 350;
	rcDoor.y = 126;

	/* chao */
	rcFloor.h = floor->h;
	rcFloor.w = floor->w;

	gameover = 0; // usada pra sair

	/* message pump */
	while (!gameover)
	{
		SDL_WaitEvent(NULL); /* para nao consumir 100% do cpu */
		SDL_Event event;

		/* /!\ ACHTUNG: DOOR /!\ */
		if (rcDoor.x == 180) {
			event.type = SDL_USEREVENT;
			event.user.code = 1; // 1 = Chegou se à porta
			event.user.data1 = 0;
			event.user.data2 = 0;
			SDL_PushEvent(&event);
		}

		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		fprintf(stderr,"Porta: x %d , y %d",rcDoor.x,rcDoor.y); 

		// draw the background
		for (x = 0; x < SCREEN_WIDTH / BACK_SIZE; x++) {
			for (y = 0; y < SCREEN_HEIGHT / BACK_SIZE; y++) {
				rcBackground.x = x * BACK_SIZE;
				rcBackground.y = y * BACK_SIZE;
				SDL_BlitSurface(background, NULL, screen, &rcBackground);
			}
		}

		/* the floor line *purple fag* */
		for (x = 0; x < SCREEN_WIDTH / floor->w; x++) {
				rcFloor.x= x * floor->w;
				rcFloor.y = 262;
				SDL_BlitSurface(floor, NULL, screen, &rcFloor);
		}

		/* draw the door */
		SDL_BlitSurface(door, &rcSrcDoor, screen, &rcDoor);

		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(door);
	SDL_FreeSurface(background);
	SDL_Quit();

	return 0;
}
