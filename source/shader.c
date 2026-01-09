#include "logger.h"
#include "shader.h"
#include "mesh.h"

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

// do i need to refactor this? there are multiple shaders for different purposes and
// certainly each one would name the attributes differently.
// probably i need to introduce a type in the shCreateFromFile function which
// sets different names based on the type.
// types: enum ShaderType
//  MESH_SHADER
//  GLYPH_SHADER
//  ???
static const char *shVariableNames[] = {
    [MESH_ATTRIBUTE_POSITION] = "inPosition",
    [MESH_ATTRIBUTE_COLOR]    = "inColor",
    [MESH_ATTRIBUTE_UV]       = "inUV",
};

static const char *readShaderFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
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

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        fclose(file);
        ERROR("failed to allocate memory for shader file: %s\n", filename);
        return NULL;
    }

    int readCount = fread(buffer, 1, length, file);
    if (readCount < length || readCount == 0)
    {
        fclose(file);
        ERROR("read returned %i which is either 0 or less than %li", readCount, length);
        return NULL;
    }

    buffer[length] = '\0';
    if (fclose(file))
    {
        ERROR("fclose failed\n");
        return NULL;
    }

    return buffer;
}

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
    ERROR("failed to compile shader: %s: %s\n", filepath, infoLog);
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
    ERROR("failed to link shader program: %s\n", infoLog);
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
        ERROR("failed to allocate memory for shader\n");
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
    const char *vsource = readShaderFile(vpath);

    if (!vsource)
        return 1;

    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vsource, NULL);
    glCompileShader(vshader);

    free((void *) vsource);

    if (shCompiledSuccessfully(vshader, vpath))
        return 1;

    /* read and compile fragment shader */
    const char *fsource = readShaderFile(fpath);

    if (!fsource)
        return 1;

    unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fsource, NULL);
    glCompileShader(fshader);

    free((void *) fsource);

    if (shCompiledSuccessfully(fshader, fpath))
        return 1;

    /* create shader program */
    unsigned int sprogram = glCreateProgram();
    if (sprogram == 0)
    {
        ERROR("failed to create shader program\n");
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
        ERROR("no uniform named '%s' found in shader->program\n", name)

    return location;
}
