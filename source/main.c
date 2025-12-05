#include "window.h"

#include "glad/glad.h"

#include <stdlib.h>

int main()
{
    struct window* window = window_create(
        800,
        600,
        "OpenGL",
        (vec4) { 0.2f, 0.3f, 0.3f, 1.0f }
    );

    if (!window)
        return EXIT_FAILURE;

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
