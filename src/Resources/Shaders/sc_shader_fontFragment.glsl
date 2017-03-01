/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_fontFragment.glsl
    ------------------------------------

	A fragment shader for rendering 2D fonts.

*/

#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D fontTexture;
uniform vec4 fontColor;

void main()
{
    color = vec4(1.0, 1.0, 1.0, texture(fontTexture, vec2(texCoord.x, texCoord.y)).r) * fontColor;
}