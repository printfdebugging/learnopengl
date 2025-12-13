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

API_CALL struct Window* winCreate(
    unsigned int width,
    unsigned int height,
    const char*  title,
    vec4         color
);

API_CALL void winSetClearColor(
    struct Window* window,
    vec4           color
);

API_CALL void winProcessInput(struct Window* window);
API_CALL void winPollEvents(struct Window* window);
API_CALL void winClearColor(struct Window* window);
API_CALL void winSwapBuffers(struct Window* window);
API_CALL void winDestroy(struct Window* window);
API_CALL bool winClosed(struct Window* window);

#endif
