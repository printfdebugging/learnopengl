#version 300 es

precision mediump float;

in vec3 inPosition;
in vec3 inColor;

out vec3 color;

void main()
{
    gl_Position = vec4(inPosition.xyz, 1.0);
    color       = inColor;
}
