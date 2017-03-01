/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_spriteFragment.glsl
    ------------------------------------

	A fragment shader for rendering 2D sprites.

*/

#version 330 core

in vec2 texCoord;

out vec4 color;

uniform float texCoordScaleX;
uniform float texCoordScaleY;
uniform sampler2D sprite;

void main()
{
    color = texture(sprite, vec2(texCoord.x * texCoordScaleX, texCoordScaleY - (texCoord.y * texCoordScaleY)));
}