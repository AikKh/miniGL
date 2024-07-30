#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 color;
uniform vec3 lightPos;

void main()
{
    float ambient = 0.1;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 result = (ambient + diff) * color;

    FragColor = vec4(result, 1.0);
}