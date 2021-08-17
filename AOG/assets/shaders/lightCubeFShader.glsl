#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// Texture sampler
uniform sampler2D glowstoneTex;

void main()
{
	FragColor = texture(glowstoneTex, TexCoords) * vec4(1.0); // bright white color
}