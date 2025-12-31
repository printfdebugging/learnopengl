#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  cursorPosition;
uniform vec2  canvasDimentions;
uniform float canvasWidth;
uniform float canvasHeight;
uniform float time;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

out vec4 colorOut;

void main()
{
    vec2 st = vec2(
        gl_FragCoord.x / canvasDimentions.x,
        gl_FragCoord.y / canvasDimentions.y
    );
    colorOut = vec4(st.xy, 0.0, 1.0);
}
