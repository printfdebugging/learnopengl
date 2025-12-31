#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 inPosition;

void main()
{
    gl_Position = vec4(inPosition.xyz, 1.0);
}
