uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 inPosition;
in vec3 inColor;
in vec2 inUV;

out vec3 color;
out vec2 uv;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition.xyz, 1.0);
    color       = inColor;
    uv          = inUV;
}
