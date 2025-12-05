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

    printf("hello, world\n");

    window_destroy(window);
    return 0;
}
