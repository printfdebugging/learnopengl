#version 330 core

/*
    variables leaving the graphics pipeline
    are annotated with _out at the end.
*/
out vec4 color_out;

in vec4 color;
in vec2 uv;

// one cool thing about uniform variables is that you don't have to add them
// to the vertex shader first, they can directly be used in the fragment shader.
uniform sampler2D TEXTURE0;
uniform sampler2D TEXTURE1;

void main()
{
    color_out = mix(texture(TEXTURE0, uv), texture(TEXTURE1, uv), 0.2) * vec4(color.xyz, 1.0);
}
