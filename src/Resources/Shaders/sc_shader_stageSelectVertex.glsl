/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_stageVertex.glsl
    ------------------------------------

	A vertex shader for rendering the stage

*/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 5) in vec3 idColor;

uniform mat4 PVW;

out vec3 fragIdColor;

void main()
{
	gl_Position = PVW * vec4(position, 1.0f);
	fragIdColor = idColor;
}