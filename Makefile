paradise:	main.c
	@gcc `sdl-config --cflags --libs` main.c -o paradise -lSDL_ttf

clean:
	@rm -f paradise
