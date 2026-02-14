
uniform vec3  material_ambient;
uniform vec3  material_diffuse;
uniform vec3  material_specular;
uniform float material_shininess;

uniform vec3 light_position;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specuar;

uniform vec3 camera_position;

in vec3 position;
in vec3 color;
in vec2 uv;
in vec3 normal;

out vec4 out_color;

void main()
{
    vec3 ambient = vec3(1.0) * light_ambient * material_ambient;

    vec3  norm            = normalize(normal);
    vec3  light_direction = normalize(light_position - position);
    float diff            = max(dot(norm, light_direction), 0.0);
    vec3  diffuse         = vec3(1.0) * light_diffuse * (diff * material_diffuse);

    vec3  view_direction    = normalize(position - camera_position);
    vec3  reflect_direction = reflect(-light_direction, normal);
    float spec              = pow(max(dot(view_direction, reflect_direction), 0.0), material_shininess);
    vec3  specular          = vec3(0.1) * light_specuar * (spec * material_specular);

    vec3 result = ambient + diffuse + specular;

    out_color = vec4(result.rgb, 1.0);
}
