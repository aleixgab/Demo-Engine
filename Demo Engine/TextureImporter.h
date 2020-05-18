#ifndef __TEXTURE_IMPORTER_H__
#define __TEXTURE_IMPORTER_H__

#include <string>

struct TextureInfo
{
	std::string name;
	unsigned int id = 0u;
};

class TextureImporter
{
public:
	TextureImporter() {}

	TextureImporter(const char* path);

	bool ImportTexture(const char* path);

	~TextureImporter();


public:
	TextureInfo texture;
};

#endif
