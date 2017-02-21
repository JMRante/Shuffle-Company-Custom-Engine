/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_flatColorFragment.glsl
    ------------------------------------

	A fragment shader for rendering flat, single color triangles

*/

#version 330 core

out vec4 color;

uniform vec4 flatColor;

void main()
{
    color = flatColor;
}