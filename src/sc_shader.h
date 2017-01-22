/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader.h
    ------------------------------------

	This is a definition of a shader class. It represents a GLSL shader used by Shuffle Company's OpenGL renderer.
	The shader can be loaded and compiled from a set of vertex and fragment shaders in the singular "build" function.
	Afterwards, it can be set to use in the OpenGL state.

*/

#ifndef SC_SHADER
#define SC_SHADER

#include <iostream>
#include <GL/glew.h>

#include "sc_utility.h"

class Shader
{
public:
	GLuint programID;

	Shader();
	bool build(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath);
	void remove();
	void use();
};

#endif