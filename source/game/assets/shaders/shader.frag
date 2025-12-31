#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2      canvasDimensions;
uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

out vec4 colorOut;

float plot(vec2 st)
{
    return smoothstep(0.02, 0.0, abs(st.y - st.x));
}

void main()
{
    vec2  st    = vec2(gl_FragCoord.xy / canvasDimensions);
    float y     = st.x; /* 1:1 relation between y and x (brightness & x)*/
    vec3  color = vec3(y);

    float a     = plot(st);
    vec3  green = vec3(0.0f, 1.0f, 0.0f);
    color       = a * green + ((1.0f - a) * color);

    colorOut = vec4(color, 1.0);
}
