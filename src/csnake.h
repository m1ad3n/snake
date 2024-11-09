#ifndef CSNAKE_HEADER
#define CSNAKE_HEADER

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TITLE "csnake"
#define WIDTH 600
#define HEIGHT 500
#define SNAKE_BODY_SIZE 10

typedef struct snake_body {
	SDL_Rect rect;
	struct snake_body* next;
} snake_body_t;

typedef struct {
	struct snake_body* head;

	// sdl releated vars
	SDL_Color color;

	// pref
	unsigned short points;
	unsigned char direction;
} snake_t;

typedef struct {
	int width;
	int height;

	SDL_Window* wptr;
	SDL_Renderer* rptr;
	SDL_Event event;
	SDL_bool quit;
	TTF_Font* font;

	snake_t* snake;
	SDL_Rect fruit;

} swindow_t;

enum SNAKE_DIRECTIONS {
	SD_NONE = 0,
	SD_LEFT,
	SD_RIGHT,
	SD_UP,
	SD_DOWN,
};

swindow_t* swindow_create(int _width, int _height);
void swindow_check_events(swindow_t* sw);
void swindow_new_fruit(swindow_t* sw);
void swindow_render_snake(swindow_t* sw);
int swindow_check_collision(swindow_t* sw);
void swindow_gameloop(swindow_t* sw);
snake_body_t* snake_body_create(int posx, int posy);
snake_t* snake_create(int posx, int posy, SDL_Color color);
void snake_grow(snake_t* sn);
void snake_move(snake_t* sn);
void snake_body_destroy(struct snake_body* sb);
void snake_destroy(snake_t* sn);
void swindow_destroy(swindow_t* sw);

#endif // CSNAKE_HEADER
