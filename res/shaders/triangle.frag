#version 330 core
in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D woodTexture;
uniform sampler2D faceTexture;

void main()
{
	FragColor = mix(texture(woodTexture, texCoords), texture(faceTexture, texCoords), 0.2) * vec4(color, 1.0f);
}