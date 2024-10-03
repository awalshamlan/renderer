#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"

bool is_running = false;


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

void render(void){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();
	clear_color_buffer(0xFF000000);
	draw_dot_grid(0xFFA0A0A0, 10);
	draw_pixel(20, 20, 0xFFFFFF00);
	draw_rect(0xFFFF0000, 300, 500, (int) window_width / 2 , (int) window_height / 2);
	SDL_RenderPresent(renderer);
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
