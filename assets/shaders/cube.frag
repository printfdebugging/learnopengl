uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

in vec3 color;
in vec2 uv;

out vec4 colorOut;

void main()
{
    colorOut = vec4(mix(
        texture(faceTexture, uv),
        texture(containerTexture, uv),
        0.2
    ));
}
