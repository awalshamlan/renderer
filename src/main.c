#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int window_width = 800;
int window_height = 600;
bool is_running = false;
uint32_t* color_buffer = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;


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


void setup(void){
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height	
	);
}

void process_input(void){
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: // when you close the window
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
	}
}

void update(void){
	// TODO:
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

void render(void){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();
	clear_color_buffer(0xFF000000);
	draw_dot_grid(0xFFA0A0A0, 10);
	draw_rect(0xFFFF0000, 300, 500, (int) window_width / 2 , (int) window_height / 2);
	SDL_RenderPresent(renderer);
}

void destroy_window(void){
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void){
	is_running = initialize_window();

	setup();

	while(is_running){
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}
