#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 inPosition;
in vec3 inColor;
in vec2 inUV;

void main()
{
    gl_Position = vec4(inPosition.xyz, 1.0);
}
