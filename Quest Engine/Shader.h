#pragma once
/************
By: Ian Solis
QUEST_ENGINE
*************/

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int programID;

	//constructors
	Shader();
	Shader(const char* vertFilename, const char* fragFilename);
	Shader(const char* vertFilename, const char* geoFilename, const char* fragFilename);

	//add vert, frag or geo shader
	int addShader(const char* filename, GLuint shaderType);

	//use this shader
	void use();
	//stop using the shader
	void unuse();

	//utility functions, set uniforms
	//scalars
	void setBool(const std::string& name, bool val) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float val) const;

	//vectors
	void setVec2(const std::string& name, glm::vec2& value) const;
	void setVec3(const std::string& name, glm::vec3& value) const;
	void setVec4(const std::string& name, glm::vec4& value) const;

	//matrices
	void setMat2(const std::string& name, glm::mat2& value) const;
	void setMat3(const std::string& name, glm::mat3& value) const;
	void setMat4(const std::string& name, glm::mat4& value) const;

private:
	void checkCompileErrors(unsigned int shaderID, std::string type);
};