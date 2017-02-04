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

uniform mat4 transform;
uniform mat4 camera;

void main()
{
	gl_Position = camera * transform * vec4(position, 1.0f);
}