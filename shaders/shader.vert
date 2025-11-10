#version 330 core

/*
    variables entering the graphics pipeline
    are annotated with in_. variables that only
    exist within the pipeline aren't annotated.
*/
in vec4 in_position;
in vec3 in_color;
in vec2 in_uv;

out vec4 color;
out vec2 uv;

void main()
{
    gl_Position = vec4(in_position.xyz, 1.0);
    color       = vec4(in_color.xyz, 1.0);
    uv          = in_uv;
}