uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

in vec3 position;
in vec3 color;
in vec2 uv;
in vec3 normal;

out vec4 out_color;

float ambient_strength = 0.1;
float specular_strength = 0.5;

void main()
{
    vec3 ambient = ambient_strength * light_color;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - position);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 view_direction = normalize(position - camera_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;

    out_color = vec4(result.rgb, 1.0);
}
