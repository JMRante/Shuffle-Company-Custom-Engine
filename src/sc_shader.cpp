/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader.cpp
    ------------------------------------

	This is a definition of a shader class. It represents a GLSL shader used by Shuffle Company's OpenGL renderer.
	The shader can be loaded and compiled from a set of vertex and fragment shaders in the singular "build" function.
	Afterwards, it can be set to use in the OpenGL state.

*/

#include "sc_shader.h"

Shader::Shader()
{
	programID = 0;
}

bool Shader::build(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath)
{
	//Has shader already been built?
	if (programID != 0)
	{
		std::cout << "Shader already built" << std::endl;		
		return false;		
	}

	//Load Shader
	GLint success;
	GLchar infoLog[512];

	//Load shader files
	const GLchar* vertexShaderSource = sc_file_read(vertexShaderFilePath);

	if (vertexShaderSource == NULL)
	{
		std::cout << "Failed to open vertex shader" << std::endl;		
		return false;
	}

	const GLchar* fragmentShaderSource = sc_file_read(fragmentShaderFilePath);

	if (fragmentShaderSource == NULL)
	{
		std::cout << "Failed to open fragment shader" << std::endl;		
		return false;
	}

	//Pass-through, Screen Space Vertex
	GLuint vertexShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Compile Error: " << infoLog << std::endl;
		return false;
	}

	//Basic Fragment
	GLuint fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compile Error: " << infoLog << std::endl;
		return false;
	}

	//Link shaders into program
	programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "Shader Program Linking Error: " << infoLog << std::endl;
		return false;		
	}

	//Clean-up Shader Code from memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Shader::remove()
{
	glDeleteProgram(programID);
	programID = 0;	
}

void Shader::use()
{
	glUseProgram(programID);
}