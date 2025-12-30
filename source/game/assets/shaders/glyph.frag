#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec2 txCoords;

out vec4 color;

uniform sampler2D glyphTexture;
uniform vec3      glyphColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyphTexture, txCoords).r);
    color        = vec4(glyphColor, 1.0) * sampled;
}
