#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int id;
	
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	
	void setVec3f(const std::string& name, float x, float y, float z) const;
	void setVec3f(const std::string& name, const glm::vec3& values) const;

	void setMat4f(const std::string& name, const glm::mat4& mat) const;
};