#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"

#include <stdbool.h>

struct window
{
    unsigned int width;
    unsigned int height;
    const char*  title;
    GLFWwindow*  window;
};

struct window* window_create(
    unsigned int width,
    unsigned int height,
    const char*  title
);

void window_process_input(struct window* window);
void window_poll_events(struct window* window);
void window_clear_color(struct window* window);
void window_swap_buffers(struct window* window);
void window_destroy(struct window* window);
bool window_closed(struct window* window);

#endif
