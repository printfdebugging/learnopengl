#include "shader.h"
#include "window.h"

#include <stdlib.h>

int main()
{
    struct window* window = window_create(800, 600, "OpenGL");
    if (!window) return EXIT_FAILURE;

    struct shader* shader = shader_create_from_file("shaders/shader.vert", "shaders/shader.frag");
    if (!shader) return EXIT_FAILURE;

    while (!window_closed(window))
    {
        window_poll_events(window);
        window_process_input(window);
        window_clear_color(window);

        window_swap_buffers(window);
    }

    window_destroy(window);
    return 0;
}
