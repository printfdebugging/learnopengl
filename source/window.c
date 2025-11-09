#include "glad/glad.h"

#include "window.h"
#include "logger.h"

#include <stdlib.h>

static void framebuffer_resize_callback(GLFWwindow* window,
                                        int         width,
                                        int         height)
{
    (void) window;
    glViewport(0, 0, width, height);
}

struct window* window_create(unsigned int width,
                             unsigned int height,
                             const char*  title)
{
    if (!glfwInit())
    {
        ERROR("failed to initialize glfw");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
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

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    struct window* win = malloc(sizeof(struct window));
    if (!win)
    {
        ERROR("failed to initialize glad\n");
        glfwTerminate();
        return NULL;
    }

    win->width  = width;
    win->height = height;
    win->title  = title;
    win->window = window;
    return win;
}

void window_process_input(struct window* window)
{
    if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->window, GLFW_TRUE);
}

void window_poll_events(struct window* window)
{
    (void) window;
    glfwPollEvents();
}

void window_clear_color(struct window* window)
{
    (void) window;
    glClear(GL_COLOR_BUFFER_BIT);
}

void window_swap_buffers(struct window* window)
{
    glfwSwapBuffers(window->window);
}

void window_destroy(struct window* window)
{
    glfwDestroyWindow(window->window);
    glfwTerminate();
}

bool window_closed(struct window* window)
{
    if (!window->window) return GL_TRUE;
    return glfwWindowShouldClose(window->window);
}