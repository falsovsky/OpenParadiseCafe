#testing
CFLAGS=	-Wall -W -Waggregate-return -Wbad-function-cast -Wcast-align	\
	-Wcast-qual -Wconversion -Wendif-labels -Wformat=2		\
	-Wformat-extra-args -Wformat-y2k -Winline			\
	-Wmissing-declarations -Wmissing-format-attribute		\
	-Wmissing-noreturn -Wmissing-prototypes -Wnested-externs	\
	-Wpacked -Wpointer-arith -Wredundant-decls -Wshadow		\
	-Wsign-compare -Wstrict-prototypes -Wundef -Wunreachable-code	\
	-Wwrite-strings -ansi -pedantic -O2 -fomit-frame-pointer -pipe

paradise:	main.c
	$(CC) $(CFLAGS) `sdl-config --cflags --libs` -lSDL_ttf $< -o $@

clean:
	rm -f paradise
