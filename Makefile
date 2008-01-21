paradise:	main.c
	@gcc `sdl-config --cflags --libs` main.c -o paradise

clean:
	@rm -f paradise
