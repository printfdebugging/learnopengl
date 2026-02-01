uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_position;
in vec3 in_color;
in vec2 in_uv;
in vec3 in_normal;

out vec3 position;
out vec3 color;
out vec2 uv;
out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(in_position.xyz, 1.0);

    position = vec3(model * vec4(in_position, 1.0));
    color = in_color;
    uv = in_uv;
    normal = in_normal;
}
