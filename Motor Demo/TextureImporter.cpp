#include "TextureImporter.h"

#include <glad/glad.h>
#include <STB/stb_image.h>
#include <iostream>

TextureImporter::TextureImporter(const char* path)
{
	ImportTexture(path);
}

bool TextureImporter::ImportTexture(const char* path)
{
	bool ret = true;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		ret = false;
	}
	stbi_image_free(data);

	std::string name = path;
	texture.name = name.substr(name.find_last_of("\\") + 1);
	
	return ret;
}

TextureImporter::~TextureImporter()
{

}

