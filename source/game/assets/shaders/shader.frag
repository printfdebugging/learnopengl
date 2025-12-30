#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 color;
in vec2 uv;

out vec4 colorOut;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

void main()
{
    colorOut = mix(texture(containerTexture, uv), texture(faceTexture, uv), 0.2);
}
