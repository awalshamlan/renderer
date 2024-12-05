#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"


#define N_POINTS (9*9*9)
vec3_t cube_points[N_POINTS]; // 9x9x9 cube
vec2_t projected_points[N_POINTS];
float x_modifier;
float y_modifier;
float z_modifier;

float fov_factor = 128;

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

	// from -1 to 1
	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25){
		for (float y = -1; y <= 1; y += 0.25){
			for (float z = 0; z <= 2; z += 0.25){
				vec3_t new_point = {.x = x, .y = y, .z = z};
				cube_points[point_count++] = new_point;
			}
		}
	}
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
			if (event.key.keysym.sym == SDLK_LEFT)
				x_modifier = 0.1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				x_modifier = -0.1;
			if (event.key.keysym.sym == SDLK_DOWN)
				z_modifier = -0.1;
			if (event.key.keysym.sym == SDLK_UP)
				z_modifier = 0.1;
			break;

	}
}

vec2_t project(vec3_t point){
	vec2_t projected_point = {
		.x = (fov_factor * point.x),
		.y = (fov_factor * point.y)
	};
	return projected_point;
}

vec2_t perspective_project(vec3_t point){
	vec2_t projected_point = {
		.x = (fov_factor * point.x * point.z),
		.y = (fov_factor * point.y * point.z)
	};
	return projected_point;
}

void update(void){
	for (int i = 0; i < N_POINTS; i++){
		vec3_t point = cube_points[i];
		if (x_modifier != 0){
			point.x = point.x + x_modifier;
		}
		if (y_modifier != 0){
			point.y = point.y + y_modifier;
		}
		if (z_modifier != 0){
			point.z = point.z + z_modifier;
		}
		cube_points[i] = point;
		vec2_t projected_point = perspective_project(point);
		projected_points[i] = projected_point;
	}
	y_modifier = 0;
	z_modifier = 0;
	x_modifier = 0;
}

void render(void){
	draw_dot_grid(0xFFA0A0A0, 10);

	for(int i = 0; i < N_POINTS; i++){
		vec2_t projected_point = projected_points[i];
		draw_rect(
			0xFFFFFF00,
			4,
			4,
			projected_point.x + (window_width / 2),
			projected_point.y + (window_height / 2)
		);
	}

	render_color_buffer();
	clear_color_buffer(0xFF000000);

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
