/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_testFragment.glsl
    ------------------------------------

	A fragment shader for experimentation.

*/

#version 330 core
in vec3 ourColor;

out vec4 color;

void main()
{
	color = vec4(ourColor, 1.0f);
}