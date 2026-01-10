#include "logger.h"
#include "shader.h"
#include "mesh.h"
#include "utils.h"

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

static const char *shVariableNames[] = {
    [MESH_ATTRIBUTE_POSITION] = "inPosition",
    [MESH_ATTRIBUTE_COLOR]    = "inColor",
    [MESH_ATTRIBUTE_UV]       = "inUV",
};

#if defined(EMSCRIPTEN)
static const char *version = "#version 300 es\n";
#else
static const char *version        = "#version 330 core\n";
static const char *floatPrecision = "#ifdef GL_ES\n"
                                    "precision mediump float;\n"
                                    "#endif\n";
#endif

static int shCompiledSuccessfully(unsigned int shader,
                                  const char  *filepath)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
        return 0;

    int infoLogLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    char infoLog[infoLogLen];
    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
    fprintf(stderr, "failed to compile shader: %s: %s\n", filepath, infoLog);
    return 1;
}

static int shLinkedSuccessfully(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
        return 0;

    int infoLogLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

    char infoLog[infoLogLen];
    glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
    fprintf(stderr, "failed to link shader program: %s\n", infoLog);
    return 1;
}

static void shBindVariableNames(unsigned int program)
{
    for (enum MeshAttribute i = MESH_ATTRIBUTE_POSITION; i < MESH_ATTRIBUTE_COUNT; ++i)
        glBindAttribLocation(program, i, shVariableNames[i]);
}

struct Shader *shCreate()
{
    struct Shader *shader = malloc(sizeof(struct Shader));
    if (!shader)
    {
        fprintf(stderr, "failed to allocate memory for shader\n");
        return NULL;
    }

    *shader = (struct Shader) { 0 };
    return shader;
}

int shLoadFromFile(struct Shader *shader,
                   const char    *vpath,
                   const char    *fpath)
{
    /* read and compile vertex shader */
    struct String *vsource = strCreate(NULL);
    if (!vsource)
        return 1;
    if (strAppend(vsource, version))
        return 1;
    if (strAppend(vsource, floatPrecision))
        return 1;
    if (strAppendFile(vsource, vpath))
        return 1;

    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, (const char **) &vsource->data, NULL);
    glCompileShader(vshader);
    strDestroy(vsource);

    if (shCompiledSuccessfully(vshader, vpath))
        return 1;

    /* read and compile fragment shader */
    struct String *fsource = strCreate(NULL);
    if (!fsource)
        return 1;
    if (strAppend(fsource, version))
        return 1;
    if (strAppend(fsource, floatPrecision))
        return 1;
    if (strAppendFile(fsource, fpath))
        return 1;

    unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, (const char **) &fsource->data, NULL);
    glCompileShader(fshader);
    strDestroy(fsource);

    if (shCompiledSuccessfully(fshader, fpath))
        return 1;

    /* create shader program */
    unsigned int sprogram = glCreateProgram();
    if (sprogram == 0)
    {
        fprintf(stderr, "failed to create shader program\n");
        return 1;
    }

    glAttachShader(sprogram, vshader);
    glAttachShader(sprogram, fshader);

    /* bind attribute locations and link */
    shBindVariableNames(sprogram);
    glLinkProgram(sprogram);

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    if (shLinkedSuccessfully(sprogram))
        return 1;

    glUseProgram(sprogram);

    shader->program = sprogram;
    return 0;
}

void shDestroy(struct Shader *shader)
{
    glDeleteProgram(shader->program);
    free(shader);
}

int shGetUniformLocation(const struct Shader *shader,
                         const char          *name)
{
    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, name);
    if (location == -1)
        fprintf(stderr, "no uniform named '%s' found in shader->program\n", name);

    return location;
}
