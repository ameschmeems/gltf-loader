#version 330 core

uniform sampler2D texture_diffuse1;

in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture_diffuse1, texCoords);
}