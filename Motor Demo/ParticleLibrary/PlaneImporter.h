#ifndef __PLANE_IMPORTER_H__
#define __PLANE_IMPORTER_H__

class PlaneImporter
{
public:
	PlaneImporter();
	PlaneImporter(unsigned int maxParticles);

	~PlaneImporter();


public:
	unsigned int VBO[6];

	unsigned int VAO = 0u;
};

#endif