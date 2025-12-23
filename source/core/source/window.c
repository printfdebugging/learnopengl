#include "glad/glad.h"

#include "core/window.h"
#include "core/logger.h"

#include <stdlib.h>
#include <string.h>
#ifdef EMSCRIPTEN
    #include <emscripten/emscripten.h>
    #include "GLFW/emscripten_glfw3.h"
#endif

static void winFrameBufResizeCallback(GLFWwindow* window,
                                      int         width,
                                      int         height)
{
    (void) window;
    glViewport(0, 0, width, height);
}

/*
 * NOTE: `emscripten_glfw_set_next_window_canvas_selector` is required
 *       if creating more than one windows. it's a TODO.
 */
struct Window* winCreate(unsigned int width,
                         unsigned int height,
                         const char*  title,
                         vec4         color)
{
    if (!glfwInit())
    {
        ERROR("failed to initialize glfw");
        return NULL;
    }

#if EMSCRIPTEN  // set WEBGL=2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        (int) width,
        (int) height,
        title,
        NULL,
        NULL
    );

    if (!window)
    {
        ERROR("failed to create glfw winodw\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        ERROR("failed to initialize glad\n");
        glfwTerminate();
        return NULL;
    }

    glfwSetFramebufferSizeCallback(window, winFrameBufResizeCallback);
    glEnable(GL_DEPTH_TEST);

    // TODO: this is explained in the blending section
    // https://learnopengl.com/Advanced-OpenGL/Blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    struct Window* win = malloc(sizeof(struct Window));
    if (!win)
    {
        ERROR("failed to initialize glad\n");
        glfwTerminate();
        return NULL;
    }

    win->width  = width;
    win->height = height;
    win->title  = title;
    memcpy(win->color, color, sizeof(float) * 4);
    win->window            = window;
    win->drawFrameCallback = NULL;
    return win;
}

void winSetClearColor(struct Window* window,
                      vec4           color)
{
    memcpy(window->color, color, sizeof(float) * 4);
}

void winProcessInput(struct Window* window)
{
    if (glfwGetKey(window->window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->window, GLFW_TRUE);
}

void winPollEvents(struct Window* window)
{
    (void) window;
    glfwPollEvents();
}

void winClearColor(struct Window* window)
{
    (void) window;
    glClearColor(
        window->color[0],
        window->color[1],
        window->color[2],
        window->color[3]
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void winSwapBuffers(struct Window* window)
{
    glfwSwapBuffers(window->window);
}

void winDestroy(struct Window* window)
{
    glfwDestroyWindow(window->window);
    glfwTerminate();
}

bool winClosed(struct Window* window)
{
    if (!window->window)
        return GL_TRUE;
    return glfwWindowShouldClose(window->window);
}

void winFireMainLoop(struct Window* window,
                     void*          data)
{
#ifdef EMSCRIPTEN
    printf(
        "emscripten: v%d.%d.%d\n",
        __EMSCRIPTEN_major__,
        __EMSCRIPTEN_minor__,
        __EMSCRIPTEN_tiny__
    );
    emscripten_set_main_loop_arg(window->drawFrameCallback, data, 0, false);
#else
    while (!winClosed(window))
    {
        if (window->drawFrameCallback)
            window->drawFrameCallback(data);
    }
#endif
}

/*
 * NOTE: maybe we want a `postFrameCallback` so that the user
 * can put some tasks there, things like cleanup etc. and then
 * we can just call that here; feels a bit overkill, let's see
 */
void winPostFrameChecks(struct Window* window)
{
#ifdef EMSCRIPTEN
    if (winClosed(window))
        emscripten_cancel_main_loop();
#else
    (void) window;
#endif
}

void winRegisterDrawFrameCallback(struct Window*        window,
                                  DrawFrameCallbackFn frameCallback)
{
    if (window->drawFrameCallback != NULL)
        ERROR("frameCallback already set")
    window->drawFrameCallback = frameCallback;
}
