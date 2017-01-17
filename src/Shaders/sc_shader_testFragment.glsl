/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_testFragment.glsl
    ------------------------------------

	A fragment shader for experimentation.

*/

#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, TexCoord);
}