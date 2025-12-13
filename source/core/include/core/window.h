#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include "core/defines.h"

#include <stdbool.h>

struct Window
{
    unsigned int width;
    unsigned int height;
    const char*  title;
    GLFWwindow*  window;
    vec4         color;
};

CORE_API struct Window* winCreate(
    unsigned int width,
    unsigned int height,
    const char*  title,
    vec4         color
);

CORE_API void winSetClearColor(
    struct Window* window,
    vec4           color
);

CORE_API void winProcessInput(struct Window* window);
CORE_API void winPollEvents(struct Window* window);
CORE_API void winClearColor(struct Window* window);
CORE_API void winSwapBuffers(struct Window* window);
CORE_API void winDestroy(struct Window* window);
CORE_API bool winClosed(struct Window* window);

#endif
