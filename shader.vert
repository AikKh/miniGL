#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 transposedModel;

void main()
{
	vec4 transform = model * vec4(aPos, 1.0);
	gl_Position = projection * view * transform;
	FragPos = transform.xyz;
	Normal = transposedModel * aNormal;
}