/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_testFragment.glsl
    ------------------------------------

	A fragment shader for experimentation.

*/

#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texture_0;

void main()
{
    color = texture(texture_0, texCoord);
}