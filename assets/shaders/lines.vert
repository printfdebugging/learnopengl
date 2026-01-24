uniform mat4 view;
uniform mat4 projection;

in vec3 inPosition;

out vec3 color;

void main()
{
    gl_Position = projection * view * vec4(inPosition.xyz, 1.0);
    color       = vec3(1.0, 0.0, 0.0);
}
