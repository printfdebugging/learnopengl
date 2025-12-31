#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  cursorPosition;
uniform vec3  canvasDimensions;
uniform float canvasWidth;
uniform float canvasHeight;
uniform float time;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

out vec4 colorOut;

void main()
{
    vec2 st = vec2(
        gl_FragCoord.x / canvasDimensions.x,
        gl_FragCoord.y / canvasDimensions.y
    );
    colorOut = vec4(st.xy, 0.0, 1.0);
}
