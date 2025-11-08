#include "window.h"

int main()
{
    struct window* window = window_create(800, 600, "OpenGL");
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
