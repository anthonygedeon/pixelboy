#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// starts SDL and creates window
bool init();

// loads media contents
bool load_media();

// free media and close SDL
void free_media();

SDL_Surface* load_surface(const char* path);

SDL_Window* global_window = NULL;

SDL_Surface* global_screen_surface = NULL;

SDL_Surface* image = NULL;

SDL_Surface* global_keypress_surfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface* global_current_surface = NULL;

int main(int argc, char* args[]) {

	SDL_Renderer* renderer = SDL_CreateRenderer(global_window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!load_media()) {
			printf("Failed to load media!\n");
		} else {
		
			bool quit = false;

			SDL_Event event;

			SDL_Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.w = image->w;
			rect.h = image->h;

			while (!quit) {

				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
				}

				SDL_RenderClear(renderer);
				SDL_RenderDrawRect(renderer, &rect);
				SDL_RenderFillRect(renderer, &rect);
				SDL_RenderPresent(renderer);

				SDL_BlitSurface(image, NULL, global_screen_surface, NULL);
				SDL_UpdateWindowSurface(global_window);
			}
		}
	}

	free_media();

	return 0;
}

bool init() {
        
    bool success = true;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		global_window = SDL_CreateWindow(
				"Pixel Platformer", 
				SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, 
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);

		if (global_window == NULL) {
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
		} else {
			global_screen_surface = SDL_GetWindowSurface(global_window);
		}
	}

	return success;
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

void free_media() {
	SDL_FreeSurface(image);

	image = NULL;

	SDL_DestroyWindow(global_window);
	global_window = NULL;

	SDL_Quit();
}


SDL_Surface* load_surface(const char* path) {
	SDL_Surface* loaded_surface = SDL_LoadBMP(path);

	if (loaded_surface == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
	}

	return loaded_surface;
}


















