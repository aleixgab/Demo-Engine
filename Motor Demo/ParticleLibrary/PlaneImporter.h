#ifndef __PLANE_IMPORTER_H__
#define __PLANE_IMPORTER_H__

class PlaneImporter
{
public:
	PlaneImporter();
	PlaneImporter(unsigned int maxParticles);

	~PlaneImporter();


public:
	unsigned int VBO = 0u;
	unsigned int VBO_Position = 0u;
	unsigned int VBO_Rotation = 0u;
	unsigned int VBO_Size = 0u;
	unsigned int VBO_Color = 0u;
	unsigned int VBO_Texture = 0u;

	unsigned int VAO = 0u;
};

#endif