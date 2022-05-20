#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// loads media contents
bool load_media();

SDL_Surface* load_surface(const char* path);

SDL_Surface* global_screen_surface = NULL;

SDL_Surface* image = NULL;

SDL_Surface* global_current_surface = NULL;

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
	
		bool is_game_running = false;

		SDL_Event event;

		SDL_Rect rect = {0, 0, 24, 24};

		int velocity = 10;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

		while (!is_game_running) {

			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					is_game_running = true;
				}
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_UP) {
						rect.y -= velocity;
					}

					if (event.key.keysym.sym == SDLK_DOWN) {
						rect.y += velocity;
					}

					if (event.key.keysym.sym == SDLK_LEFT) {
						rect.x -= velocity;
					}

					if (event.key.keysym.sym == SDLK_RIGHT) {
						rect.x += velocity;
					}

				}
				
			}
		
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

			SDL_RenderDrawRect(renderer, &rect);
			SDL_RenderPresent(renderer);

			SDL_UpdateWindowSurface(window);
		}
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


bool load_media() {
	bool success = true;

	image = IMG_Load("assets/Characters/character_0000.png");
	if (image == NULL) {
		printf("Unable to laod image %s! SDL Error: %s\n", ".png", SDL_GetError());
		success = false;
	}

	return success;
}

SDL_Surface* load_surface(const char* path) {
	SDL_Surface* loaded_surface = SDL_LoadBMP(path);

	if (loaded_surface == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
	}

	return loaded_surface;
}


















