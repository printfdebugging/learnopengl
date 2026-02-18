#ifndef SHADER_H
#define SHADER_H

struct shader {
    unsigned int program;
};

struct shader *shader_create();

void shader_destroy(struct shader *shader);
int  shader_load_from_file(struct shader *shader, const char *vpath, const char *fpath);
int  shader_get_uniform_location(const struct shader *shader, const char *name);

#define shader_set_uniform(shader, name, type, ...)                     \
    {                                                                   \
        int var_##location = shader_get_uniform_location(shader, name); \
        if (var_##location != -1)                                       \
            glUniform##type(var_##location, __VA_ARGS__);               \
    }

#endif
