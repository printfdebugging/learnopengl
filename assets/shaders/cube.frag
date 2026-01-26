uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

uniform bool useTextures;
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 color;
in vec2 uv;

out vec4 colorOut;

void main()
{
    if (useTextures)
    {
        colorOut = vec4(mix(
            texture(faceTexture, uv),
            texture(containerTexture, uv),
            0.2
        ));
    }
    else
    {
        colorOut = vec4(lightColor * objectColor, 1.0);
    }
}
