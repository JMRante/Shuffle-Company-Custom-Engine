/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_fontVertex.glsl
    ------------------------------------

	A vertex shader for rendering 2D fonts.

*/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 PVW;

out vec2 texCoord;

void main()
{
	gl_Position = PVW * vec4(position, 1.0f);
	texCoord = textureCoord;
}