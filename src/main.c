#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))

const int SCREEN_WIDTH = 940;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;

const int CHARACTER_W = 24;
const int CHARACTER_H = 24;

SDL_Surface* load_surface(const char* path);

SDL_Rect objects[] = {
	{0, 0, 24, 24}, 
	{0, SCREEN_HEIGHT-24, SCREEN_WIDTH, 1},
};

void update();

void draw();

bool is_collision(SDL_Rect* objects);

bool STATE_IDLE = false;
bool STATE_RUNNING = false;

/**
*/
enum KeyCode {
	Up, Down, Left, Right
};

bool key_state[4];

/** Handle Vector positioning on the x-axis and y-axis
*/
typedef struct Vector2D {
	// x and y direction
	int64_t x, y;
} Vector2D;

/**
*/
typedef struct sprite_sheet {
	// individual frames of the sprite sheet
	SDL_Texture* frame;

	// acts as a reference of what sprite we actually want to show
	SDL_Rect  srcrect;
	
	// what to put on the the rect
	SDL_Rect dstrect;
	
	// state for the orientation for the sprite
	// @TODO this field does not belong in this struct
	SDL_RendererFlip orientation;

} sprite_sheet;

 /** Divides the sprite width to the total width of the sprite sheet.
  *
  * @param sprite - sprite_sheet struct that holds data for sprite_sheet
  * @return total frames in sprite sheet
 */
uint64_t calculate_total_frames(sprite_sheet* sprite) {
	int width;
	int query = SDL_QueryTexture(sprite->frame, NULL, NULL, &width, NULL);
	if (query != 0) {
		printf("Unable to query texture! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	return width / CHARACTER_W;
}

// @TODO checks collision of all objects in object[]
bool is_collision(SDL_Rect* objects) {
	return SDL_HasIntersection(&objects[0], &objects[1]);
}

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Rect srcrect_idle = {0, 0, 24, 24};

	SDL_Renderer* renderer = NULL;

	SDL_Window* window = SDL_CreateWindow(
			"Pixel Platformer", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);


	SDL_Event event;

	Vector2D velocity = {5, 10};
	Vector2D gravity = {0, 10};

	int max_jump = 300;

	bool in_air = false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	sprite_sheet sprite = {
		IMG_LoadTexture(renderer, "assets/Characters/character_0000.png"),
	};

	bool running = false;
	while (!running) {

		uint32_t ticks = SDL_GetTicks();
		uint32_t sprite_target = (ticks / 100) % calculate_total_frames(&sprite);

		sprite.srcrect.x = sprite_target * 24;
		sprite.srcrect.y = 0;
		sprite.srcrect.w = 24;
		sprite.srcrect.h = 24;

		while (SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_w:
							key_state[Up] = true;
							break;
						case SDLK_s:
							key_state[Down] = true;
							break;
						case SDLK_a:
							key_state[Left] = true;
							sprite.orientation = SDL_FLIP_NONE;
							break;
						case SDLK_d:
							key_state[Right] = true;
							sprite.orientation = SDL_FLIP_HORIZONTAL;
							break;
					}
					break;

				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_w:
							key_state[Up] = false;
							break;
						case SDLK_s:
							key_state[Down] = false;
							break;
						case SDLK_a:
							key_state[Left] = false;
							break;
						case SDLK_d:
							key_state[Right] = false;
							break;
					}
					break;

				case SDL_QUIT:
					running = true;
					break;
			}
		}
		if (in_air) {
			key_state[Up] = false;

		}

		if (key_state[Up]) {
			gravity.y = 0;
			in_air = true;
		} 

		if (key_state[Down]) {
			objects[0].y += gravity.y; 
		}

		if (key_state[Left]) {
			objects[0].x -= velocity.x;
			STATE_RUNNING = true;
			STATE_IDLE = false;
		}

		if (key_state[Right]) {
			objects[0].x += velocity.x;
			STATE_RUNNING = true;
			STATE_IDLE = false;
		}

		if (objects[0].y < max_jump) {
			gravity.y = 10;
		}

		if (gravity.y != 0) {
			objects[0].y += gravity.y;
		} else {
			objects[0].y -= 10;
		}

		if (is_collision(objects)) {
			objects[0].y = objects[1].y-24;
			in_air = false;
		}

		if (key_state[Up] == 0 
				&& key_state[Down] == 0 
				&& key_state[Left] == 0 
				&& key_state[Right] == 0) {
			STATE_IDLE = true;
			STATE_RUNNING = false;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		// render objects
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		for (int i = 0; i < LEN(objects); i++) {
			SDL_RenderDrawRect(renderer, &objects[i]);
		}

		if (STATE_RUNNING) {
			SDL_RenderCopyEx(renderer, sprite.frame, &sprite.srcrect, &objects[0], 0, 0, sprite.orientation);
		}

		if (STATE_IDLE) {
			SDL_RenderCopyEx(renderer, sprite.frame, &srcrect_idle, &objects[0], 0, 0, sprite.orientation);
		}

		SDL_RenderPresent(renderer);

		SDL_Delay(20);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

