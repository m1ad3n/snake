#include "csnake.h"
#include <stdlib.h>

int time_passed(unsigned int ms) {
	static unsigned int time;
	if (!time) {
		time = SDL_GetTicks();
		return 1;
	}

	if (SDL_GetTicks() - time >= ms) {
		time = SDL_GetTicks();
		return 1;
	}

    return 0;
}

swindow_t* swindow_create(int _width, int _height) {
	swindow_t* sw = (swindow_t*)malloc(sizeof(swindow_t));
	if (!sw) return NULL; // malloc failed

	// init sdl library
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		free(sw);
		return NULL;
	}

	// sdl window
	sw->wptr = SDL_CreateWindow(TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_width, _height, SDL_WINDOW_RESIZABLE);

	// sdl renderer
	sw->rptr = SDL_CreateRenderer(sw->wptr, 0, SDL_RENDERER_PRESENTVSYNC);
	
	// font init
	if (TTF_Init() < 0) {
		swindow_destroy(sw);
		return NULL;
	}

	sw->font = TTF_OpenFont("assets/font.otf", 20);
	if (!sw->font) {
		swindow_destroy(sw);
		return NULL;
	}

	sw->width = _width;
	sw->height = _height;
	sw->quit = SDL_FALSE;

	// init random seed
	// srand(time(NULL));
	sw->fruit.w = SNAKE_BODY_SIZE;
	sw->fruit.h = SNAKE_BODY_SIZE;
	swindow_new_fruit(sw);

	return sw;
}

void swindow_new_fruit(swindow_t* sw) {
	sw->fruit.x = rand() % ((sw->width - 50) - 50 + 1) + 50;
	sw->fruit.y = rand() % ((sw->height - 50) - 60 + 1) + 60;

	sw->fruit.x -= (sw->fruit.x % SNAKE_BODY_SIZE);
	sw->fruit.y -= (sw->fruit.y % SNAKE_BODY_SIZE);
}

void swindow_check_events(swindow_t* sw) {
	while (SDL_PollEvent(&sw->event)) {
		switch (sw->event.type) {
		case SDL_QUIT:
			sw->quit = SDL_TRUE;
			break;

		case SDL_KEYUP:
			switch (sw->event.key.keysym.sym) {
			case SDLK_LEFT:
				if (sw->snake->direction != SD_RIGHT)
					sw->snake->direction = SD_LEFT;
				break;

			case SDLK_RIGHT:
				if (sw->snake->direction != SD_LEFT)
					sw->snake->direction = SD_RIGHT;
				break;

			case SDLK_UP:
				if (sw->snake->direction != SD_DOWN)
					sw->snake->direction = SD_UP;
				break;

			case SDLK_DOWN:
				if (sw->snake->direction != SD_UP)
					sw->snake->direction = SD_DOWN;
				break;

			case SDLK_ESCAPE:
				sw->quit = SDL_TRUE;
				break;
			}

			break;
		}	
	}
}

void swindow_render_snake(swindow_t* sw) {
	snake_body_t* tmp = sw->snake->head;
	SDL_SetRenderDrawColor(sw->rptr,
		sw->snake->color.r, sw->snake->color.g,
		sw->snake->color.b, sw->snake->color.a
	);
	while (tmp) {
		// SDL_RenderDrawRect(sw->rptr, &tmp->rect);
		SDL_RenderFillRect(sw->rptr, &tmp->rect);
		tmp = tmp->next;
	}
}

void swindow_gameloop(swindow_t* sw) {
	if (!sw) return;

	char score_text[50] = {0};
	SDL_Surface* score_surface = TTF_RenderText_Blended(sw->font, "Score: 0", (SDL_Color){255, 255, 255, 255});
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(sw->rptr, score_surface);

	while (!sw->quit) {
		// check window events
		swindow_check_events(sw);
		
		// clear the screen
		SDL_SetRenderDrawColor(sw->rptr, 0, 0, 0, 255);
		SDL_RenderClear(sw->rptr);
		
		// move the snake
		if (time_passed(40)) snake_move(sw->snake);
		
		int cc = swindow_check_collision(sw);
		if (cc == 1) { // fruit eaten
			snake_grow(sw->snake);
			swindow_new_fruit(sw);
			sw->snake->points++;

			SDL_FreeSurface(score_surface);
			SDL_DestroyTexture(score_texture);

			sprintf(score_text, "Score: %d", sw->snake->points);
			score_surface = TTF_RenderText_Blended(sw->font, score_text, (SDL_Color){255, 255, 255, 255});
			score_texture = SDL_CreateTextureFromSurface(sw->rptr, score_surface);
		}
		else if (cc == 2) {
			sw->snake->direction = SD_NONE;

			SDL_FreeSurface(score_surface);
			SDL_DestroyTexture(score_texture);

			sprintf(score_text, "Score: %d\t\tYou lost", sw->snake->points);
			score_surface = TTF_RenderText_Blended(sw->font, score_text, (SDL_Color){255, 255, 255, 255});
			score_texture = SDL_CreateTextureFromSurface(sw->rptr, score_surface);
		}
		
		// render the snake
		swindow_render_snake(sw);

		// render the fruit
		SDL_SetRenderDrawColor(sw->rptr, 255, 0, 0, 255);
		SDL_RenderFillRect(sw->rptr, &sw->fruit);

		// render the score
		SDL_RenderCopy(sw->rptr, score_texture, NULL, &(SDL_Rect){
			10, 10,
			score_surface->w, score_surface->h
		});

		// render the top line
		SDL_SetRenderDrawColor(sw->rptr, 255, 255, 255, 255);
		SDL_RenderDrawLine(sw->rptr, 0, 50, sw->width, 50);

		// show rendered stuff on the screen
		SDL_RenderPresent(sw->rptr);
	}

	SDL_FreeSurface(score_surface);
	SDL_DestroyTexture(score_texture);
}

int swindow_check_collision(swindow_t* sw) {
	SDL_Rect snake_head = sw->snake->head->rect; 
	if (snake_head.x < sw->fruit.x + sw->fruit.w &&
		snake_head.x + snake_head.w > sw->fruit.x &&
		snake_head.y < sw->fruit.y + sw->fruit.h &&
    	snake_head.y + snake_head.h > sw->fruit.y)
			return 1; // fruit eaten

	// check for screen bounds
	if (snake_head.x < 0 || snake_head.x > sw->width)
		return 2;
	else if (snake_head.y < 50 || snake_head.y > sw->height)
		return 2;

	snake_body_t* tmp = sw->snake->head->next;
	while (tmp) {
		if (SDL_RectEquals(&tmp->rect, &sw->snake->head->rect))
			return 2;
		tmp = tmp->next;
	}

	return 0;
}

snake_body_t* snake_body_create(int posx, int posy) {
	snake_body_t* sb = (snake_body_t*)malloc(sizeof(snake_body_t));
	if (!sb) return NULL; // malloc failed
	sb->rect.x = posx;
	sb->rect.y = posy;
	sb->rect.w = SNAKE_BODY_SIZE;
	sb->rect.h = SNAKE_BODY_SIZE;
	sb->next = NULL;
	return sb;
}

snake_t* snake_create(int posx, int posy, SDL_Color color) {
	snake_t* sn = (snake_t*)malloc(sizeof(snake_t));
	if (!sn) return NULL; // malloc failed
	sn->head = snake_body_create(posx, posy);
	sn->color = color;
	sn->points = 0;
	return sn;
}

void snake_grow(snake_t* sn) {
	snake_body_t* tail = sn->head;
	while (tail->next)
		tail = tail->next;
	tail->next = snake_body_create(tail->rect.x + 1, tail->rect.y);
}

void snake_move(snake_t* sn) {
	if (sn->direction == SD_NONE) return;
	// positions for the new head
	int new_head_posx = sn->head->rect.x;
	int new_head_posy = sn->head->rect.y;

	switch (sn->direction) {
	case SD_LEFT:
		new_head_posx -= SNAKE_BODY_SIZE * 0.7;
		break;
	case SD_RIGHT:
		new_head_posx += SNAKE_BODY_SIZE * 0.7;
		break;
	case SD_UP:
		new_head_posy -= SNAKE_BODY_SIZE * 0.7;
		break;
	case SD_DOWN:
		new_head_posy += SNAKE_BODY_SIZE * 0.7;
		break;
	}

	snake_body_t* new_head_sb = snake_body_create(
		new_head_posx, new_head_posy
	);

	if (sn->points)
		new_head_sb->next = sn->head;
	else
		snake_body_destroy(sn->head);
	sn->head = new_head_sb;

	// remove the last item of the linked list (tail)
	snake_body_t* tail = NULL;
	if (sn->points) {
		tail = sn->head;
		while (tail->next->next != NULL) {
			tail = tail->next;
		}
		snake_body_destroy(tail->next);
		tail->next = NULL;
	}
}

void snake_body_destroy(struct snake_body* sb) {
	if (!sb) return;
	snake_body_destroy(sb->next);
	free(sb);
}

void snake_destroy(snake_t* sn) {
	if (!sn) return;
	snake_body_destroy(sn->head);
	free(sn);
}

void swindow_destroy(swindow_t* sw) {
	if (!sw) return;
	SDL_DestroyRenderer(sw->rptr);
	SDL_DestroyWindow(sw->wptr);
	snake_destroy(sw->snake);
	free(sw);
}

int main(int argc, char* argv[]) {
	swindow_t* window = swindow_create(WIDTH, HEIGHT);
	snake_t* snake = snake_create(
		(WIDTH / 2) - SNAKE_BODY_SIZE, (HEIGHT / 2) - SNAKE_BODY_SIZE, (SDL_Color){255, 255, 0, 255}
	);
	window->snake = snake;

	// run the game
	swindow_gameloop(window);

	// deallocate the memory
	swindow_destroy(window);
	return 0;
}