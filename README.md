# learnopengl

- learn how to use renderdoc.
- textures can also be used to store large amounts of data to be sent to the shader.
- figure out why the shaders need modification to work with wasm
  - browsers don't use opengl, they use webgl and webgl is based on opengl es,
  so it's not really
- call js from c and c from js

## TODO
- CMakePresets.json

## Installation Instructions

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_DIR=./install ..
make -j 32 
make install
```

## Custom Target

- add it to the applicaiton's cmake while development and bind `cmake --build build -t run` in
  you editor (if it's vim or emacs) to quickly run the application. do not commit this as that
  doesn't make much sense.

```cmake
add_custom_target(run
    COMMAND ${APPLICATION}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${APPLICATION}
    COMMENT "Running ${APPLICATION} from root directory"
)
```
