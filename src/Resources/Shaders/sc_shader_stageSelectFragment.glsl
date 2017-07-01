/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_stageFragment.glsl
    ------------------------------------

	A fragment shader for rendering the stage

*/

#version 330 core

in vec3 fragIdColor;

out vec4 color;

void main()
{
    color = vec4(fragIdColor, 1.0f);
}