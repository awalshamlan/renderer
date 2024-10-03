#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;


int window_width = 800;
int window_height = 600;

bool initialize_window(void){
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr, "Error intializing SDL.\n");
		return false;
	}

	// Query fullscreen resolution
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_FULLSCREEN
	);
	if (!window){
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer){
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}
	//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

	return true;
}

void render_color_buffer(void){
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int) window_width * sizeof(uint32_t)
	);
	SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	);
}

void draw_dot_grid(uint32_t color, int spacing){
	for (int y = 0; y < window_height; y += spacing){
		for (int x = 0; x < window_width; x += spacing){
			color_buffer[(y * window_width) + x] = color;
		}
	}
}


void draw_grid(uint32_t color, int spacing){
	for (int y = 0; y < window_height; y++){
		for (int x = 0; x < window_width; x++){
			if(y % spacing == 0 || x % spacing == 0){
				color_buffer[(y * window_width) + x] = color;
			}
		}
	}
}

void draw_rect(uint32_t color, int len_x, int len_y, int pos_x, int pos_y){
	for (int y = pos_y; (y < pos_y+len_y && y != window_height); y++){
		for (int x = pos_x; (x < pos_x+len_x && x != window_width); x++){
			color_buffer[(y * window_width) + x] = color;
		}
	}
}


void clear_color_buffer(uint32_t color){
	for (int y = 0; y < window_height; y++){	
		for (int x = 0; x < window_width; x++){
			color_buffer[(y * window_width) + x] = color;
		}
	}
}


void destroy_window(void){
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
