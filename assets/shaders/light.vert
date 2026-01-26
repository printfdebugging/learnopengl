uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 inPosition;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition.xyz, 1.0);
}
