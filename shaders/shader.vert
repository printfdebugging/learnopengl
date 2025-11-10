#version 330 core

in vec4 in_position;
in vec3 in_color;

out vec4 color;

void main()
{
    gl_Position = vec4(in_position.xyz, 1.0);
    color       = vec4(in_color.xyz, 1.0);
}