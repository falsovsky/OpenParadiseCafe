/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

/* this game can never fail */
#include <SDL.h>
#include <SDL_ttf.h>

#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define SPRITE_WIDTH		40
#define SPRITE_HEIGHT		128

#define DOOR_SIZE		400

#define BACK_SIZE		32

#define USEREVENT_REDRAW	0
#define USEREVENT_DOOR		1

int gameover;

/* source and destination rectangles */
SDL_Rect rcSrc, rcSprite;
SDL_Rect rcSrcDoor, rcDoor;

int i;

/* always-walk timer */

SDL_TimerID jewish_timer = NULL;

struct jewish_param_t {
	SDLKey lol;
} jewish_param;

Uint32 jewish_timer_callback(Uint32 interval, void *param)
{
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = ((struct jewish_param_t *)param)->lol;
	SDL_PushEvent(&event);
	return interval;
}

/* door timer */

int door_closed = 1;	/* to make sure the door only opens once */

struct door_param_t {
	int frame;
	SDL_TimerID timer;
} door_param;

Uint32 door_timer_callback(Uint32 interval, void *param)
{
	SDL_Event event;
	int frame = ((struct door_param_t *)param)->frame;
	rcSrcDoor.x = frame*56;
	if(++frame > 4)
		SDL_RemoveTimer(((struct door_param_t *)param)->timer);
	else
		((struct door_param_t *)param)->frame = frame;
	event.type = SDL_USEREVENT;
	event.user.code = USEREVENT_REDRAW;
	SDL_PushEvent(&event);
	return interval;
}

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
					rcSrc.x = (rcSrc.x + 40)%160;
					rcDoor.x += 10;
					break;
				case SDLK_RIGHT:
					rcSrc.x = (rcSrc.x + 40)%160; /* sprite rectangle movement */
					rcDoor.x -= 10; /* door movement */
/*
					if (rcDoor.w<=0) {
						rcDoor.w=56;
						rcDoor.x = 220;
					}
*/
					/* failed scroller */
					if (rcDoor.x<=50) {
						//rcDoor.w -= 10;
						rcDoor.w = (rcDoor.w - 10);
					}

					break;
				case SDLK_PAGEUP:
					jewish_param.lol = SDLK_RIGHT;
				case SDLK_PAGEDOWN:
					if(event.key.keysym.sym == SDLK_PAGEDOWN)
						jewish_param.lol = SDLK_LEFT;
					if(jewish_timer == NULL)
						jewish_timer = SDL_AddTimer(100, jewish_timer_callback, &jewish_param);
					else {
						SDL_RemoveTimer(jewish_timer);
						jewish_timer = NULL;
					}
					break;
			}
			break;
		case SDL_USEREVENT:
			switch (event.user.code) {
				case USEREVENT_REDRAW:
					break;
				case USEREVENT_DOOR:
					printf("wtc\n\n\n");
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
	SDL_Surface *screen, *temp, *sprite, *door, *floor; 
	SDL_Rect rcFloor, rcBackground;/* background e chaozinho roxo */
	/* text stuff */
	TTF_Font *fntText;
	SDL_Rect rcText = { 20, 50, 0,0 };
	SDL_Color clrText = { 0,0,0, 0 };
	char debug[20];
	SDL_PixelFormat *fmt; 
	int colorkey;
	int x,y;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	/* initialize SDL_ttf */
	if(TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}

	/* set the title bar */
	SDL_WM_SetCaption("Open Paradise Cafe", "Open Paradise Cafe");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	fmt=screen->format; // get pixelformat

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(70, 70);

	/* load sprite damatta */
	temp = SDL_LoadBMP("sprites.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

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
	rcSrcDoor.w = 56; /* so a primeira frame */
	rcSrcDoor.h = door->h;
	
	/* posicao inicial da porta */
	rcDoor.x = 350;
	rcDoor.y = 126;

	/* chao */
	rcFloor.x=0;
	rcFloor.y=262;
	rcFloor.h = 24;
	rcFloor.w = SCREEN_WIDTH;

	/* background */
	rcBackground.x=0;
	rcBackground.y=0;
	rcBackground.w=SCREEN_WIDTH;
	rcBackground.h=SCREEN_HEIGHT;


	gameover = 0; /* usada pra sair */

	/* load a font and give it a point-size */
	fntText = TTF_OpenFont("arial.ttf", 12);
	if(!fntText) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(1);
	}

	/* message pump */
	while (!gameover)
	{
		SDL_WaitEvent(NULL); /* para nao consumir 100% do cpu */
		SDL_Event event;

		/* /!\ ACHTUNG: DOOR /!\ */
		if (rcDoor.x == 180 && door_closed) {
			door_closed = 0;
			door_param.frame = 1;
			door_param.timer = SDL_AddTimer(250, door_timer_callback, &door_param);
/*			event.type = SDL_USEREVENT;
			event.user.code = USEREVENT_DOOR;
			event.user.data1 = 0;
			event.user.data2 = 0;
			SDL_PushEvent(&event);*/
		}

		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		/* draw the background */
		SDL_FillRect(screen, &rcBackground, SDL_MapRGB(fmt,200,65,52)); 

		/* the floor line *purple fag* */
		SDL_FillRect(screen, &rcFloor, SDL_MapRGB(fmt,198,0,198)); 

		/* draw the door */
		SDL_BlitSurface(door, &rcSrcDoor, screen, &rcDoor);

		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* render text to an SDL_Surface */
		sprintf(debug,"door: x=%d w=%d",rcDoor.x,rcDoor.w);
		SDL_Surface *sText = TTF_RenderText_Solid(fntText, debug, clrText);
		/* blit text to the screen */
		SDL_BlitSurface(sText, NULL, screen, &rcText);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(door);
//	SDL_FreeSurface(background);
	SDL_Quit();

	return 0;
}
