#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

enum attr
{
    MESH_ATTRIBUTE_POSITION = 0,
    MESH_ATTRIBUTE_COLOR    = 1,
    MESH_ATTRIBUTE_UV       = 2,
    MESH_ATTRIBUTE_COUNT,
};

struct shader
{
    unsigned int program;
};

struct shader* shader_create_from_file(
    const char* vpath,
    const char* fpath
);

bool shader_set_uniform1i(
    const struct shader* shader,
    const char*          name,
    int                  value
);

bool shader_set_uniform_mat4fv(
    const struct shader* shader,
    const char*          name,
    float*               value
);

#endif
