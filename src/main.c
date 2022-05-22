#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int SCREEN_WIDTH = 940;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;

// loads media contents
bool load_media();

SDL_Surface* load_surface(const char* path);

SDL_Surface* global_screen_surface = NULL;

SDL_Surface* image = NULL;

SDL_Surface* global_current_surface = NULL;

/**
 *
 *
 */
enum KeyCode {
	Up, Down, Left, Right
};

bool key_state[4];

/** Handle Vector positioning on the x-axis and y-axis
 *@TODO: Possible refactor when other use cases are needed
 *@TODO: Might add more fields if the need comes
 */
typedef struct Vector2D {
	int64_t x, y;
} Vector2D;

bool load_media() {
	bool success = true;

	image = IMG_Load("assets/Characters/character_0000.png");
	if (image == NULL) {
		printf("Unable to laod image %s! SDL Error: %s\n", ".png", SDL_GetError());
		success = false;
	}

	return success;
}

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	
	// image to load comment for now
	//SDL_Surface* sprite = IMG_Load("assets/Characters/character_0000.png");

	SDL_Window* window = SDL_CreateWindow(
				"Pixel Platformer", 
				SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, 
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);


	SDL_Renderer* renderer = NULL;

	if (!load_media()) {
		printf("Failed to load media!\n");
	} else {

		uint64_t now = SDL_GetPerformanceCounter();
		uint64_t last = 0;
		uint64_t deltatime = 0;

		bool running = false;

		SDL_Event event;

		SDL_Rect rect = {0, 0, 24, 24};
		SDL_Rect rect1 = {0, SCREEN_HEIGHT-24, SCREEN_WIDTH, 1};

		Vector2D velocity = {8, 10};
		Vector2D gravity = {0, 10};

		int max_jump = 300;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		while (!running) {

			last = now;
			now = SDL_GetPerformanceCounter();
			deltatime = ((now-last)*1000.0f / (double)SDL_GetPerformanceFrequency());

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
								break;
							case SDLK_d:
								key_state[Right] = true;
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
		
			if (key_state[Up]) {
				gravity.y = 0;
			} 
			
			if (key_state[Down]) {
				rect.y += gravity.y; 
			}
			
			if (key_state[Left]) {
				rect.x -= velocity.x;
			}
			
			if (key_state[Right]) {
				rect.x += velocity.x;
			}
		
			if (rect.y < max_jump) {
				gravity.y = 10;
			}

			if (gravity.y != 0) {
				rect.y += gravity.y;
			} else {
				rect.y -= 10;
			}

			if (SDL_HasIntersection(&rect, &rect1)) {
				rect.y = rect1.y-24;
			}


			//printf("[ %i %i %i %i ]\n", key_state[0], key_state[1], key_state[2], key_state[Right]);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

			SDL_RenderClear(renderer);
		
			// render rect
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(renderer, &rect);
		
			// render rect
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(renderer, &rect1);

			SDL_RenderPresent(renderer);

			SDL_UpdateWindowSurface(window);

			SDL_Delay(20);
		}
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

