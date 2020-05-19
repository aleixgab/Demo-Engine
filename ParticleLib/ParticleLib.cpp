// ParticleLib.cpp : Define las funciones exportadas del archivo DLL.
//
#include "ParticleLib.h"

#include <iostream>

// Ejemplo de variable exportada
PARTICLELIB_API int nParticleLib=0;

// Ejemplo de función exportada.
PARTICLELIB_API void fnParticleLib(const char* text)
{
	std::cout << text << std::endl;
}

// Constructor de clase exportada.
CParticleLib::CParticleLib()
{
    return;
}
