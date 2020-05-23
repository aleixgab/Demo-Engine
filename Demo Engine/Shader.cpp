#include "Shader.h"
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
{

}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	SetShader(vertexPath, geometryPath, fragmentPath);
}
void Shader::SetShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
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
		if(geometryPath)
			gShaderFile.open(geometryPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, gShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		if (geometryPath)
			gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		if (geometryPath)
			gShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		if (geometryPath)
			geometryCode = gShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.code() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, geometry,fragment;
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

	if (geometryPath)
	{
		const char* gShaderCode = geometryCode.c_str();
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
	}

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
	uid = glCreateProgram();
	glAttachShader(uid, vertex);
	if (geometryPath)
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
	if (geometryPath)
		glDeleteShader(geometry);
	glDeleteShader(fragment);
}

void Shader::UseProgram() const
{
	glUseProgram(uid);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(uid, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(uid, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(uid, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(uid, name.c_str()), 1, &value.x);
}
void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(uid, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(uid, name.c_str()), 1, &value.x);
}
void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(uid, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(uid, name.c_str()), 1, &value.x);
}
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(uid, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(uid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(uid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(uid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

