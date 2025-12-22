#version 300 es

precision mediump float;

in vec3 color;

out vec4 triangleColor;

void main()
{
    triangleColor = vec4(color.xyz, 1.0);
}
