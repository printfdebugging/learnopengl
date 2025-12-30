#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec4 inPosition;

out vec2 txCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(inPosition.xy, 0.0, 1.0);
    txCoords    = inPosition.zw;
}
