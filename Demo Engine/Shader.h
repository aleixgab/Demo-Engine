#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:

	Shader();


	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
	//You need the constructor or call this function to add new shader
	void SetShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

	// use/activate the shader
	void UseProgram() const;
	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec2(const std::string& name, float x, float y) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w) const;

	void SetMat2(const std::string& name, const glm::mat2& mat) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	   	  
public:
	// the program ID
	unsigned int uid;
};
#endif