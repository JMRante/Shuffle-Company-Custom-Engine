/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_flatColorVertex.glsl
    ------------------------------------

	A vertex shader for rendering flat, single color triangles

*/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 PVW;

void main()
{
	gl_Position = PVW * vec4(position, 1.0f);
}