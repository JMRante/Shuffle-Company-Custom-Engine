/*

    SHUFFLE COMPANY SOURCE CODE

    sc_shader_stageFragment.glsl
    ------------------------------------

	A fragment shader for rendering the stage

*/

#version 330 core

in vec2 texCoord;
in float texNum;

out vec4 color;

uniform sampler2DArray textureArray_0;

void main()
{
    color = texture(textureArray_0, vec3(texCoord, texNum));
    //color = vec4(texNum, 0.0f, 1.0f-texNum, 1.0f);
}