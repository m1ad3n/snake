CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -g `pkg-config --cflags sdl2`
LIBS = `pkg-config --libs SDL2_ttf`

csnake: src/csnake.c src/csnake.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

.PHONY: clean
clean:
	@rm -rvf csnake