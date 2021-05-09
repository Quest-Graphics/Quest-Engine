/************
By: Ian Solis
QUEST_ENGINE
*************/

#include "Shader.h"

/********************************
ORDER OF SHADERS:
VERTEX -> GEOMETRY -> FRAGMENT
********************************/

//default
//Can add shaders later
Shader::Shader()
{
}

//vertex and fragment only
Shader::Shader(const char* vertFilename, const char* fragFilename)
{
	programID = glCreateProgram();

	int vert = addShader(vertFilename, GL_VERTEX_SHADER);
	int frag = addShader(fragFilename, GL_FRAGMENT_SHADER);

	glAttachShader(programID, vert);
	glAttachShader(programID, frag);
	glLinkProgram(programID);
	checkCompileErrors(programID, "PROGRAM");

	glDeleteShader(vert);
	glDeleteShader(frag);
}

//vertex, geometry, and fragment
Shader::Shader(const char* vertFilename, const char* geoFilename, const char* fragFilename)
{
	programID = glCreateProgram();

	int vert = addShader(vertFilename, GL_VERTEX_SHADER);
	int geo = addShader(geoFilename, GL_GEOMETRY_SHADER);
	int frag = addShader(fragFilename, GL_FRAGMENT_SHADER);

	glAttachShader(programID, vert);
	glAttachShader(programID, geo);
	glAttachShader(programID, frag);
	glLinkProgram(programID);
	checkCompileErrors(programID, "PROGRAM");

	glDeleteShader(vert);
	glDeleteShader(geo);
	glDeleteShader(frag);
}


//creates a vertex, fragment or geometry shader
//can be used individually
//returns the ID of the shader
int Shader::addShader(const char* filename, GLuint shaderType)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderBuffer;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//read the contents of the file into a buffer
	try
	{
		shaderFile.open(filename);
		shaderBuffer << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderBuffer.str();
	}
	catch(std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filename << std::endl;
	}

	const char* shaderStr = shaderCode.c_str();

	//compile shader code read in
	int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderStr, NULL);
	glCompileShader(shaderID);

	//check if shader complied successfully
	std::string type;
	switch(shaderType)
	{
	case GL_VERTEX_SHADER:
		type = "VERTEX";
		break;
	case GL_FRAGMENT_SHADER:
		type = "FRAGMENT";
		break;
	case GL_GEOMETRY_SHADER:
		type = "GEOMETRY";
		break;
	default:
		type = "ERR";
		break;
	}
	checkCompileErrors(shaderID, type);

	return shaderID;
}


//checks the compilation of each shader component
void Shader::checkCompileErrors(unsigned int shaderID, std::string type)
{
	int success;
	char infoLog[1024];

	//checks the compilation status of the shader program
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	
	//error message
	else if (type == "ERR")
	{
		std::cout << "ERROR::SHADER::" << type << "::NOT A VALID SHADER TYPE\n" << infoLog << std::endl;
	}

	//checks shader compilation status
	else
	{
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}


//use the shader
void Shader::use()
{
	glUseProgram(programID);
}

void Shader::unuse()
{
	glUseProgram(0);
}

//set uniforms
//bool
void Shader::setBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)val);
}

//int
void Shader::setInt(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), val);
}

//float
void Shader::setFloat(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
}

//vec2
void Shader::setVec2(const std::string& name, glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

//vec3
void Shader::setVec3(const std::string& name, glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

//vec4
void Shader::setVec4(const std::string& name, glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

//mat2
void Shader::setMat2(const std::string& name, glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//mat3
void Shader::setMat3(const std::string& name, glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//mat4
void Shader::setMat4(const std::string& name, glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}