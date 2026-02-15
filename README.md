# LearnOpenGL

- [ ] hot-reloading
- [ ] add widget controls to allow live-tweeking (imgui, clay or something custom)

<!-- - The tutorials are deployed at [printfdebugging.in](https://printfdebugging.in/) (check out the graphics section) -->
<!-- - Following [Learn OpenGL](https://learnopengl.com/) tutorials -->


<!-- # 2025-12-31 -->
<!--     - [ ] Implement an fps counter -->
<!--     - [ ] Performance Metrics using Timer Queries (https://wikis.khronos.org/opengl/Query_Object#Timer_queries) -->
<!--     - [ ] Load a simple glTF model with a few textures -->

<!-- TODO:  -->
<!--     - create docs/cmake.md to explain the cmake lessons -->
<!--     - purge the modules and unnecessary wrappers -->
<!--     - just have one executable. -->

## Project Structure

The project uses external libraries cloned as git submodules in
`/libs` directory. Common project assets like logo are stored in
`/assets`, CMake specific files live in `/cmake` and these include
header only projects, config template files, graph generation steps
etc. `/source` contains all the source files

The project is organized into modules like `source/core`,
`source/loader`, `source/tutorials`. `core` provides the core
functionality like windowing and logging while `loader` as the name
suggests, provids the loading functionality. `core` and `loader` are
built as libraries used by executable modules in the `tutorials`
module.

```
 assets
 cmake
 libs
 scripts
 source
│  core
│ │  include
│ │  source
│ │  CMakeLists.txt
│  loader
│ │  include
│ │  source
│ │  CMakeLists.txt
│ ...
│  tutorials
│ │  hello-triangle
│ │ │  assets
│ │ │  source
│ │ │  CMakeLists.txt
│ │ │ 󰂺 README.md
│ │ ...
 CMakeLists.txt
```

<!-- ### Emscripten Specific Details -->
<!---->
<!-- The cmake build files use `if (EMSCRIPTEN)` conditional blocks to -->
<!-- separate normal build commands from wasm specific commands. Each -->
<!-- tutorial generates three files `index.{html,js,wasm}`. These are -->
<!-- copied over to `prefix/bin/${tutorial}` directory during installation. -->
<!---->
<!-- Tutorial specific asset files live in the tutorial's local `assets` -->
<!-- directory while common assets live in the `/assets` directory. Both -->
<!-- can be accessed from within the build tutorials using `ASSETS_DIR` and -->
<!-- `COMMON_ASSETS_DIR` definitions respectively. This way asset location -->
<!-- can be changed just by changing the build files. -->
<!---->
<!-- Code Usage: -->
<!---->
<!-- ```c -->
<!-- shader = shCreateFromFile( -->
<!--     ASSETS_DIR "shaders/shader.vert", -->
<!--     ASSETS_DIR "shaders/shader.frag" -->
<!-- ); -->
<!-- ``` -->
<!---->
<!-- Wasm builds use the `scripts/index.html` shell file by default unless -->
<!-- `USE_GENERATED_INDEX_HTML:BOOLEAN` `GENERATED_GRAPHICS_DIR_PATH:STRING` -->
<!-- are defined. These allow us to use a different shell file. You can also -->
<!-- see each tutorial has a `README.md` file with a similar header.  -->
<!---->
<!-- ```md -->
<!-- --- -->
<!-- title: Hello Triangle -->
<!-- date: 2025-12-17 -->
<!-- --- -->
<!---->
<!-- {{ include(path="/canvas.md") }} -->
<!---->
<!-- This is some dummy text, should be removed later. -->
<!-- ``` -->
<!---->
<!-- This `README.md` file is copied over to my [website] which then uses -->
<!-- `zola` (a static site generator) to generate a new shell file. Then -->
<!-- the wasm/html files are generated using the generated shell file & -->
<!-- finally everything is copied & deployed on the website. See [scripts] -->
<!-- and the [gitlab-ci] file for more details. -->
<!---->
<!-- [zola website repo]: https://gitlab.com/printfdebugging/website -->
<!-- [scripts]: https://gitlab.com/printfdebugging/website/-/tree/main/scripts?ref_type=heads -->
<!-- [gitlab-ci]: https://gitlab.com/printfdebugging/website/-/blob/main/.gitlab-ci.yml?ref_type=heads -->

## Dependencies

See [dependencies.sh](./scripts/dependencies.sh), it's a shell script
to install all the required dependencies on archlinux and MSYS2
(Winodws). I will later create a `dependencies.bat` file to install
dependencies on windows.

## How to Build

```bash
cd learnopengl
cmake -B build -S .                    # for normal build
emcmake cmake -B build -S .            # for wasm build
cmake --build build -j $(nproc)        # build
cmake --install build --prefix install # install
```

## Naming Convention

Single word variables are named as usual like `texture` or `shader`
but when there are multiple words involved, we use the shrinked words
in camel case, like `txUnits` or `shVarName`. This is applicable for
both variable and function names. For entities like `mesh`, the base
name is already not that long, so we don't shorten that further.

TODO: we need a proper naming convention for the shader variables,
- uniforms
- vertex attributes
- internal variables

# shaders

- https://www.shadertoy.com/ -- shader examples
- https://thebookofshaders.com -- shaders 101
- https://github.com/septag/dmon -- shader hot reloading
