#ifndef SHADER_H
#define SHADER_H

struct shader
{
    unsigned int program;
};

struct shader *shader_create();
void shader_destroy(struct shader *shader);
int shader_load_from_file(struct shader *shader, const char *vpath, const char *fpath);
int shader_get_uniform_location(const struct shader *shader, const char *name);

#endif
