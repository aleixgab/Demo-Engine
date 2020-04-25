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
	unsigned int VBO_StaticCoord = 0u;
	unsigned int VBO_TransformX = 0u;
	unsigned int VBO_TransformY = 0u;
	unsigned int VBO_TransformZ = 0u;
	unsigned int VBO_TransformW = 0u;
	unsigned int VBO_Color = 0u;
	unsigned int VBO_Texture = 0u;

	unsigned int VAO = 0u;
};

#endif