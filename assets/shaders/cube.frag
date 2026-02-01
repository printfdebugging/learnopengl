uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;

in vec3 position;
in vec3 color;
in vec2 uv;
in vec3 normal;

out vec4 out_color;

void main()
{
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - position);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse) * object_color;

    out_color = vec4(result.rgb, 1.0);
}
