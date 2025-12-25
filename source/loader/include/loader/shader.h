#ifndef SHADER_H
#define SHADER_H

#include "loader/defines.h"

struct Shader
{
    unsigned int program;
};

// TODO: add a shCreate() function to just create a shader object
// and change the below function to shLoadFromFile. then add another
// function shLoadFromMemory... and make these functions return int
// instead of struct pointer, except for the constructor & destructor
// (which returns void atleast for now)

LOADER_API struct Shader *shCreate();

LOADER_API void shDestroy(struct Shader *shader);
LOADER_API void shBind(const struct Shader *shader);
LOADER_API void shUnbind(const struct Shader *shader);

LOADER_API int shLoadFromFile(
    struct Shader *shader,
    const char    *vpath,
    const char    *fpath
);

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
