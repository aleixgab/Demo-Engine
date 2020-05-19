// El siguiente bloque ifdef muestra la forma estándar de crear macros que hacen la exportación
// de un DLL más sencillo. Todos los archivos de este archivo DLL se compilan con PARTICLELIB_EXPORTS
// símbolo definido en la línea de comandos. Este símbolo no debe definirse en ningún proyecto
// que use este archivo DLL. De este modo, otros proyectos cuyos archivos de código fuente incluyan el archivo verán
// interpretan que las funciones PARTICLELIB_API se importan de un archivo DLL, mientras que este archivo DLL interpreta los símbolos
// definidos en esta macro como si fueran exportados.
#ifdef PARTICLELIB_EXPORTS
#define PARTICLELIB_API __declspec(dllexport)
#else
#define PARTICLELIB_API __declspec(dllimport)
#endif

// Clase exportada del DLL
class PARTICLELIB_API CParticleLib {
public:
	CParticleLib(void);
	// TODO: agregar métodos aquí.
};

extern PARTICLELIB_API int nParticleLib;

PARTICLELIB_API void fnParticleLib(const char* text);
