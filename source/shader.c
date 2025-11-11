#include "glad/glad.h"

#include "logger.h"
#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const char* program_variables[] = {
    [MESH_ATTRIBUTE_POSITION] = "in_position",
    [MESH_ATTRIBUTE_COLOR]    = "in_color",
    [MESH_ATTRIBUTE_UV]       = "in_uv",
};

static const char* read_shader_file(const char* filename)
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

static bool shader_compiled_successfully(unsigned int shader,
                                         const char*  filepath)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
        return true;

    int info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

    char info_log[info_log_length];
    glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
    ERROR("failed to compile shader: %s: %s\n", filepath, info_log);
    return false;
}

static bool shader_program_linked_successfully(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
        return true;

    int info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

    char info_log[info_log_length];
    glGetProgramInfoLog(program, info_log_length, NULL, info_log);
    ERROR("failed to link shader program: %s\n", info_log);
    return false;
}

static void shader_bind_variable_names(unsigned int shader_program)
{
    for (enum attr i = MESH_ATTRIBUTE_POSITION; i < MESH_ATTRIBUTE_COUNT; ++i)
        glBindAttribLocation(shader_program, i, program_variables[i]);
}

struct shader* shader_create_from_file(const char* vertex_path,
                                       const char* fragment_path)
{
    /* read and compile vertex shader */
    const char* vertex_source = read_shader_file(vertex_path);

    if (!vertex_source)
        return NULL;

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    free((void*) vertex_source);

    if (!shader_compiled_successfully(vertex_shader, vertex_path))
        return NULL;

    /* read and compile fragment shader */
    const char* fragment_source = read_shader_file(fragment_path);

    if (!fragment_source)
        return NULL;

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    free((void*) fragment_source);

    if (!shader_compiled_successfully(fragment_shader, fragment_path))
        return NULL;

    /* create shader program */
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    /* bind attribute locations and link */
    shader_bind_variable_names(shader_program);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    if (!shader_program_linked_successfully(shader_program))
        return NULL;

    glUseProgram(shader_program);

    struct shader* shader = malloc(sizeof(struct shader));
    if (!shader)
    {
        ERROR("failed to allocate shader\n");
        return NULL;
    }

    shader->program = shader_program;
    return shader;
}
