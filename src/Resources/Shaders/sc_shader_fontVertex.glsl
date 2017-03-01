/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_spriteVertex.glsl
    ------------------------------------

	A vertex shader for rendering 2D sprites.

*/

#version 330 core
layout (location = 0) in vec4 positionAndTextureCoord;

uniform mat4 PVW;

out vec2 texCoord;

void main()
{
	gl_Position = PVW * vec4(positionAndTextureCoord.xy, 0.0f, 1.0f);
	texCoord = positionAndTextureCoord.zw;
}