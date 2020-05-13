#ifndef __Plane_Importer_H__
#define __Plane_Importer_H__

class PlaneImporter
{
public:
	PlaneImporter();
	PlaneImporter(unsigned int maxParticles);

	~PlaneImporter();


public:
	unsigned int VBO[11];

	unsigned int VAO = 0u;
};

#endif