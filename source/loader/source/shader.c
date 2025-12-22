#include "glad/glad.h"
#include "core/logger.h"

#include "loader/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const char* shVariableNames[] = {
    [MESH_ATTRIBUTE_POSITION] = "inPosition",
    [MESH_ATTRIBUTE_COLOR]    = "inColor",
    [MESH_ATTRIBUTE_UV]       = "inUV",
};

static const char* readShaderFile(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        ERROR("failed to read shader file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length < 0)
    {
        fclose(file);
        ERROR("failed to get the shader file's length: %s\n", filename);
        return NULL;
    }

    char* buffer = malloc(length + 1);
    if (!buffer)
    {
        fclose(file);
        ERROR("failed to allocate memory for shader file: %s\n", filename);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    return buffer;
}

static bool shCompiledSuccessfully(unsigned int shader,
                                   const char*  filepath)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
        return true;

    int infoLogLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    char infoLog[infoLogLen];
    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
    ERROR("failed to compile shader: %s: %s\n", filepath, infoLog);
    return false;
}

static bool shLinkedSuccessfully(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
        return true;

    int infoLogLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

    char infoLog[infoLogLen];
    glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
    ERROR("failed to link shader program: %s\n", infoLog);
    return false;
}

static void shBindVariableNames(unsigned int program)
{
    for (enum MeshAttribute i = MESH_ATTRIBUTE_POSITION; i < MESH_ATTRIBUTE_COUNT; ++i)
    {
        glBindAttribLocation(program, i, shVariableNames[i]);
        glEnableVertexAttribArray(i);
    }
}

struct Shader* shCreateFromFile(const char* vpath,
                                const char* fpath)
{
    /* read and compile vertex shader */
    const char* vsource = readShaderFile(vpath);

    if (!vsource)
        return NULL;

    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vsource, NULL);
    glCompileShader(vshader);

    free((void*) vsource);

    if (!shCompiledSuccessfully(vshader, vpath))
        return NULL;

    /* read and compile fragment shader */
    const char* fsource = readShaderFile(fpath);

    if (!fsource)
        return NULL;

    unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fsource, NULL);
    glCompileShader(fshader);

    free((void*) fsource);

    if (!shCompiledSuccessfully(fshader, fpath))
        return NULL;

    /* create shader program */
    unsigned int sprogram = glCreateProgram();
    glAttachShader(sprogram, vshader);
    glAttachShader(sprogram, fshader);

    /* bind attribute locations and link */
    shBindVariableNames(sprogram);
    glLinkProgram(sprogram);

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    if (!shLinkedSuccessfully(sprogram))
        return NULL;

    glUseProgram(sprogram);

    struct Shader* shader = malloc(sizeof(struct Shader));
    if (!shader)
    {
        ERROR("failed to allocate shader\n");
        return NULL;
    }

    shader->program = sprogram;
    return shader;
}

bool shUniform1i(const struct Shader* shader,
                 const char*          name,
                 int                  value)
{
    int location = glGetUniformLocation(shader->program, name);
    if (location == -1)
    {
        ERROR("no uniform named '%s' found in shader->program\n", name);
        return false;
    }

    glUniform1i(location, value);
    return true;
}

bool shUniformMatrix4fv(const struct Shader* shader,
                        const char*          name,
                        float*               value)
{
    int location = glGetUniformLocation(shader->program, name);
    if (location == -1)
    {
        ERROR("no uniform named '%s' found in shader->program\n", name);
        return false;
    }

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        value
    );

    return true;
}
