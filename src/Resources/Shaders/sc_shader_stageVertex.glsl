/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_stageVertex.glsl
    ------------------------------------

	A vertex shader for rendering the stage

*/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;
layout (location = 3) in float textureNum;
layout (location = 4) in int id;

uniform mat4 PVW;
uniform int int_0;

out vec2 texCoord;
out float texNum;
out float highlight;

void main()
{
	gl_Position = PVW * vec4(position, 1.0f);
	texCoord = textureCoord;
	texNum = textureNum;
	highlight = 0.0f;

	if (id == int_0)
	{
		highlight = 0.3f;
	}
}