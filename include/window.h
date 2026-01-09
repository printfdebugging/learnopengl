#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"
#include "cglm/cglm.h"

#include <stdbool.h>

typedef void DrawFrameCallback(void *data);

struct Window
{
    unsigned int width;
    unsigned int height;
    const char  *title;
    GLFWwindow  *window;
    vec4         color;

    DrawFrameCallback *drawFrameCallback;
};

struct Window *winCreate(
    unsigned int width,
    unsigned int height,
    const char  *title,
    vec4         color
);

void winSetClearColor(
    struct Window *window,
    vec4           color
);

void winProcessInput(struct Window *window);
void winPollEvents(struct Window *window);
void winClearColor(struct Window *window);
void winSwapBuffers(struct Window *window);
void winDestroy(struct Window *window);
bool winClosed(struct Window *window);

void winFireMainLoop(
    struct Window *window,
    void          *data
);

void winPostFrameChecks(struct Window *window);

void winRegisterDrawFrameCallback(
    struct Window     *window,
    DrawFrameCallback *frameCallback
);

#endif
