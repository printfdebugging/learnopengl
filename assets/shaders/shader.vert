#version 330 core

/*
    variables entering the graphics pipeline
    are annotated with in_. variables that only
    exist within the pipeline aren't annotated.
*/
in vec4 in_position;
in vec3 in_color;
in vec2 in_uv;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =  projection * view * model * vec4(in_position.xyz, 1.0);
    uv          = in_uv;
}
