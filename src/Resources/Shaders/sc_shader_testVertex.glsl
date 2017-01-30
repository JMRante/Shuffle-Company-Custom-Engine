/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_testVertex.glsl
    ------------------------------------

	A vertex shader for experimentation.

*/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0f);
}