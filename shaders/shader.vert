#version 330 core

in vec4 in_position;

void main()
{
    gl_Position = vec4(in_position.xyz, 1.0);
}