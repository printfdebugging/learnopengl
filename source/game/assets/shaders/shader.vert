#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 inPosition;
in vec3 inColor;
in vec2 inUV;

out vec3 color;
out vec2 uv;

void main()
{
    gl_Position = vec4(inPosition.xyz, 1.0);
    color       = inColor;
    uv          = inUV;
}
