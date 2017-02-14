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

uniform mat4 PVW;

out vec2 texCoord;

void main()
{
	gl_Position = PVW * vec4(position, 1.0f);
	texCoord = textureCoord;
}