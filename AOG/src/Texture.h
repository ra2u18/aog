#pragma once

// Third Party library
#include <stb_image.h>
#include <glad/glad.h>

// System library
#include <string>

class Texture
{
private:
	struct TextureStorage {
		int width, height, nrChannels;
	};

public:
	unsigned int id;
	TextureStorage storage;

	Texture(const std::string& path);
	~Texture();

	void Bind(GLenum slot = GL_TEXTURE0) const;

private:
	void init();
	void loadImage(const std::string& path);
};