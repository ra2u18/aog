#include "Texture.h"

#include <iostream>

Texture::Texture(const std::string& path)
{
	init();
	loadImage(path);
}

Texture::~Texture() {}

void Texture::Bind(GLenum slot) const 
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::init()
{
	// Generate Texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Set texture wrap attributes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::loadImage(const std::string& path)
{
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true); // flip loaded texture's on the y-axis
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	storage.height = height;
	storage.width = width;
	storage.nrChannels = nrChannels;

	if (data) {
		GLenum internalFormat = 0, dataFormat = 0;
		if (nrChannels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (nrChannels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}