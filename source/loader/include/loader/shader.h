#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include "loader/defines.h"

struct Shader
{
    unsigned int program;
};

LOADER_API struct Shader *shCreateFromFile(
    const char *vpath,
    const char *fpath
);

LOADER_API void shDestroy(struct Shader *shader);
LOADER_API void shBind(const struct Shader *shader);
LOADER_API void shUnbind(const struct Shader *shader);

/*
 * returns 0 on success
 * returns 1 on failure
 */
LOADER_API int shUniform1i(
    const struct Shader *shader,
    const char          *name,
    int                  value
);

/*
 * returns 0 on success
 * returns 1 on failure
 */
LOADER_API int shUniform3f(
    const struct Shader *shader,
    const char          *name,
    float                first,
    float                second,
    float                third
);

/*
 * returns 0 on success
 * returns 1 on failure
 */
LOADER_API int shUniformMatrix4fv(
    const struct Shader *shader,
    const char          *name,
    float               *value
);

#endif
