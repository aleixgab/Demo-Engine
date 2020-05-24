#include <ParticleSystem.h>
#include <ParticleManager.h>

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

PARTICLELIB_API bool Part::Update(float dt)
{
	return manager->Update(dt);
}

PARTICLELIB_API void Part::StartLibrary()
{
	gladLoadGL();
	manager = new ParticleManager();
}

PARTICLELIB_API void Part::CleanUpLibrary()
{
	delete manager->cameraPos;
}

//Call this function from the renderer to draw all the particles 
PARTICLELIB_API void Part::Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw)
{
	manager->Draw(shaderProgramUuid, viewMatrix, projMatrix, emittersToDraw);
}

PARTICLELIB_API void Part::GetEmitters(std::list<ParticleEmitter*>& emitters)
{
	emitters = manager->emittersList;
}

//Create new emitter
PARTICLELIB_API ParticleEmitter* Part::CreateEmitter(float* emitterPos, int maxParticles)
{
	ParticleEmitter* newEmitter = new ParticleEmitter(emitterPos, maxParticles);
	manager->emittersList.push_back(newEmitter);

	return newEmitter;
}

//Remove the wanted emitter
PARTICLELIB_API bool Part::RemoveEmitter(ParticleEmitter* emitter)
{
	bool ret = false;
	if (emitter != nullptr)
	{
		manager->emittersList.remove(emitter);
		delete emitter;
		ret = true;
	}

	return ret;
}

PARTICLELIB_API void Part::StartAllEmitters()
{
	manager->StartAllEmitters();
}
PARTICLELIB_API void Part::StartEmmitter(ParticleEmitter* emitter)
{
	manager->StartEmmitter(emitter);
}

PARTICLELIB_API void Part::PauseAllEmitters()
{
	manager->PauseAllEmitters();
}

PARTICLELIB_API void Part::PauseEmmitter(ParticleEmitter* emitter)
{
	manager->PauseEmmitter(emitter);
}

PARTICLELIB_API void Part::StopAllEmitters()
{
	manager->StopAllEmitters();
}

PARTICLELIB_API void Part::StopEmitter(ParticleEmitter* emitter)
{
	manager->StopEmitter(emitter);
}

PARTICLELIB_API bool Part::SetCameraPos(float* newCameraPos)
{
	bool ret = false;
	if (manager->cameraPos)
	{
		manager->cameraPos[0] = newCameraPos[0];
		manager->cameraPos[1] = newCameraPos[1];
		manager->cameraPos[2] = newCameraPos[2];

		ret = true;
	}
	return ret;
}

PARTICLELIB_API int Part::SetShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		gShaderFile.open(geometryPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, gShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		gShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		geometryCode = gShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.code() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	unsigned int vertex, geometry, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	// print compile errors if any
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader Program
	int uid = glCreateProgram();
	glAttachShader(uid, vertex);
	glAttachShader(uid, geometry);
	glAttachShader(uid, fragment);
	glLinkProgram(uid);
	// print linking errors if any
	glGetProgramiv(uid, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(uid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);

	return uid;
}
