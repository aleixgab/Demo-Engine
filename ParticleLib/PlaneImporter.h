#ifndef __Plane_Importer_H__
#define __Plane_Importer_H__

class PlaneImporter
{
public:
	PlaneImporter();

	void SetDynamicValues(unsigned int maxParticles);

	~PlaneImporter();


public:
	unsigned int VBO_dynamic[2];

	unsigned int VAO = 0u;
};

#endif