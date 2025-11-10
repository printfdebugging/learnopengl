#ifndef SHADER_H
#define SHADER_H

enum mesh_attribute
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

struct shader* shader_create_from_file(const char* vertex_path,
                                       const char* fragment_path);

#endif
