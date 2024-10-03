#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;


extern int window_width;
extern int window_height;


bool initialize_window(void);
void render_color_buffer(void);
void draw_dot_grid(uint32_t color, int spacing);
void draw_grid(uint32_t color, int spacing);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(uint32_t color, int len_x, int len_y, int pos_x, int pos_y);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif
