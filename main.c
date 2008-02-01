/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details.
*
* http://www.youtube.com/watch?v=1ayOiJbH6pY
*/

/* this game can never fail */
#include <SDL_ttf.h>

#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define SPRITE_WIDTH		40
#define SPRITE_HEIGHT		128

#define DOOR_SIZE		400

#define USEREVENT_REDRAW	0
#define USEREVENT_DOOR		1

Uint32		 jewish_timer_callback(Uint32, void *);
Uint32		 door_timer_callback(Uint32, void *);
void		 HandleEvent(const SDL_Event);
SDL_Surface	*load_surface(const char *const);

int gameover;

/* source and destination rectangles */
SDL_Rect rcSrc;
SDL_Rect rcSrcDoor, rcDoor;

/* always-walk timer */
SDL_TimerID jewish_timer = NULL;

struct jewish_param_t {
	SDLKey lol;
} jewish_param;

Uint32
jewish_timer_callback(Uint32 interval, void *param)
{
	SDL_Event event;


	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = ((struct jewish_param_t *) param)->lol;
	SDL_PushEvent(&event);

	return interval;
}

int door_closed = 1;	/* to make sure the door only opens once */

/* door timer */
struct door_param_t {
	int frame;
	SDL_TimerID timer;
} door_param;

Uint32
door_timer_callback(Uint32 interval, void *param)
{
	SDL_Event event;
	const int frame = ((struct door_param_t *) param)->frame;


	rcSrcDoor.x = frame * 56;

	if (frame >= 4)
		SDL_RemoveTimer(((struct door_param_t *) param)->timer);
	else
		((struct door_param_t *) param)->frame = frame + 1;
	event.type = SDL_USEREVENT;
	event.user.code = USEREVENT_REDRAW;
	SDL_PushEvent(&event);

	return interval;
}

void
HandleEvent(const SDL_Event event)
{
	int	i;


	switch (event.type) {
		/* Close button clicked: */
		case SDL_QUIT:
			gameover = 1;
			break;

		/* Handle keyboard: */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;

				case SDLK_LEFT:
					rcSrc.x = (rcSrc.x + (160 - 40)) % 160;
					rcDoor.x += 10;

					if (rcDoor.x >= SCREEN_WIDTH)
						rcDoor.x = 0;
					break;

				case SDLK_RIGHT:
					rcSrc.x = (rcSrc.x + 40) % 160;
					rcDoor.x -= 10;

					if (rcDoor.x <= 0) {
						rcDoor.x = 0;
						rcSrcDoor.w -= 10;
						if (rcSrcDoor.w <= 10) {
							rcSrcDoor.w = 56;
							rcSrcDoor.x = 0;
							rcDoor.x = SCREEN_WIDTH;
							door_closed = 1;
						}
					}
					break;

				case SDLK_PAGEUP:
					jewish_param.lol = SDLK_RIGHT;

				case SDLK_PAGEDOWN:
					if (event.key.keysym.sym == SDLK_PAGEDOWN)
						jewish_param.lol = SDLK_LEFT;

					if (jewish_timer == NULL)
						jewish_timer = SDL_AddTimer(100, jewish_timer_callback, &jewish_param);
					else {
						SDL_RemoveTimer(jewish_timer);
						jewish_timer = NULL;
					}

				default:
					break;
			}
			break;

		case SDL_USEREVENT:
			switch (event.user.code) {
				case USEREVENT_REDRAW:
					break;

				case USEREVENT_DOOR:
					printf("wtc\n");
					if (rcSrcDoor.x == 0)
						for (i = 0; i <= 4; i++) {
							rcSrcDoor.x = i * 56;
/*
							rcSrcDoor.y = 0;
							rcSrcDoor.w = 56;
							rcSrcDoor.h = 136;
*/
						}
			}
	}
}

SDL_Surface *
load_surface(const char *const filename)
{
	SDL_Surface	*sfc = NULL;
	SDL_Surface	*tmp;


	if (filename != NULL) {
		tmp = SDL_LoadBMP(filename);
		if (tmp != NULL) {
			sfc = SDL_DisplayFormat(tmp);
			SDL_FreeSurface(tmp);
		}
	}

	return sfc;
}

int
main(int argc, char *argv[])
{
	SDL_Surface *screen, *sprite, *door;
	SDL_Rect rcSprite, rcWall, rcFloor, rcStatus;
	TTF_Font *fntText;
	SDL_Surface *sText;
	SDL_Rect rcText = { 5, 290, 0, 0 };
	SDL_Color clrText = { 192, 192, 192, 0 };
	char debug[100];
	unsigned int colorkey;


	/* Initialize the SDL library: */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("%s: SDL_Init(): %s\n", argv[0], SDL_GetError());
		exit(EXIT_FAILURE);
	}
	/* Clean up SDL on exit: */
	if (atexit(SDL_Quit) != 0) {
		printf("%s: Cannot register exit function SDL_Quit().\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Initialize SDL TTF engine: */
	if (TTF_Init() != 0) {
		printf("%s: TTF_Init(): %s\n", argv[0], TTF_GetError());
		exit(EXIT_FAILURE);
	}
	/* Clean up TTF on exit: */
	if (atexit(TTF_Quit) != 0) {
		printf("%s: Cannot register exit function TTF_Quit().\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Set up video: */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	if (screen == NULL) {
		printf("%s: SDL_SetVideoMode: %s\n", argv[0], SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Set the title bar: */
	SDL_WM_SetCaption("Open Paradise Cafe", NULL);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(70, 70);

	/* load sprite damatta */
	sprite = load_surface("sprites.bmp");

	/* load door */
	door = load_surface("porta.bmp");

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
	/*printf("sprite w=%d %d h=%d %d\n", sprite->w, SPRITE_WIDTH, sprite->h, SPRITE_HEIGHT);*/

	/* define o tamanho das portas */
	rcSrcDoor.x = 0;
	rcSrcDoor.y = 0;
	rcSrcDoor.w = 56; /* so a primeira frame */
	rcSrcDoor.h = door->h;
	/*printf("door w=%d h=%d\n", door->w, door->h);*/

	/* posicao inicial da porta */
	rcDoor.x = 350;
	rcDoor.y = 126;

	/* Wall: */
	rcWall.x = 0;
	rcWall.y = 0;
	rcWall.w = SCREEN_WIDTH;
	rcWall.h = SCREEN_HEIGHT;

	/* Floor:  */
	rcFloor.x = 0;
	rcFloor.y = 262;
	rcFloor.w = SCREEN_WIDTH;
	rcFloor.h = 24;

	/* Status: */
	rcStatus.x = 0;
	rcStatus.y = 262 + 24;
	rcStatus.w = SCREEN_WIDTH;
	rcStatus.h = SCREEN_HEIGHT - 262 - 24;

	/* load a font and give it a point-size */
	fntText = TTF_OpenFont("arial.ttf", 12);
	if (fntText == NULL) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	gameover = 0; /* usada pra sair */
	while (!gameover) {
		SDL_Event event;
		SDL_WaitEvent(NULL); /* para nao consumir 100% do cpu */

		/* /!\ ACHTUNG: DOOR /!\ */
		if (rcSprite.x + SPRITE_WIDTH == rcDoor.x && door_closed) {
			door_closed = 0;
			door_param.frame = 1;
			door_param.timer = SDL_AddTimer(250, door_timer_callback, &door_param);
/*
			event.type = SDL_USEREVENT;
			event.user.code = USEREVENT_DOOR;
			event.user.data1 = 0;
			event.user.data2 = 0;
			SDL_PushEvent(&event);
*/
		}

		/* look for an event */
		if (SDL_PollEvent(&event))
			HandleEvent(event);

		/* Draw the wall (red): */
		SDL_FillRect(screen, &rcWall, SDL_MapRGB(screen->format, 200, 65, 52));

		/* Draw the floor (purple fag): */
		SDL_FillRect(screen, &rcFloor, SDL_MapRGB(screen->format, 198, 0, 198));

		/* Draw the status line (blue): */
		SDL_FillRect(screen, &rcStatus, SDL_MapRGB(screen->format, 0, 0, 198));

		/* Draw the door: */
		SDL_BlitSurface(door, &rcSrcDoor, screen, &rcDoor);

		/* Draw the sprite: */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* render text to an SDL_Surface */
		sprintf(debug, "src(x=%d y=%d) door(x=%d y=%d w=%d h=%d) srcdoor(x=%d y=%d w=%d h=%d)",
			rcSrc.x, rcSrc.y, rcDoor.x, rcDoor.y, rcDoor.w, rcDoor.h, rcSrcDoor.x, rcSrcDoor.y, rcSrcDoor.w, rcSrcDoor.h);
		sText = TTF_RenderText_Solid(fntText, debug, clrText);
		SDL_BlitSurface(sText, NULL, screen, &rcText);

		/* Update the screen: */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* Clean up: */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(door);

	return 0;
}
