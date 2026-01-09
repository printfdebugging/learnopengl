#ifndef SHADER_H
#define SHADER_H

struct Shader
{
    unsigned int program;
};

struct Shader *shCreate();

void shDestroy(struct Shader *shader);
void shBind(const struct Shader *shader);
void shUnbind(const struct Shader *shader);

int shLoadFromFile(
    struct Shader *shader,
    const char    *vpath,
    const char    *fpath
);

int shGetUniformLocation(
    const struct Shader *shader,
    const char          *name
);

#endif
