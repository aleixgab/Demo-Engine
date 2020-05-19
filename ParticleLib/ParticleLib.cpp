// ParticleLib.cpp : Define las funciones exportadas del archivo DLL.
//

#include "pch.h"
#include "framework.h"
#include "ParticleLib.h"


// Ejemplo de variable exportada
PARTICLELIB_API int nParticleLib=0;

// Ejemplo de función exportada.
PARTICLELIB_API int fnParticleLib(void)
{
    return 0;
}

// Constructor de clase exportada.
CParticleLib::CParticleLib()
{
    return;
}
