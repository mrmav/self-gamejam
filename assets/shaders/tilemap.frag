#version 460 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform vec4 uColor;
uniform float uTime;
uniform sampler2D texture1;


void main()
{
    ivec2 texSize = textureSize(texture1, 0);
    float s_offset = (1.0 / (float(texSize.x))) * 0.5;
    float t_offset = (1.0 / (float(texSize.y))) * 0.5;
    vec2 tc_final = vec2(textureCoord.x - s_offset, textureCoord.y - t_offset);

    FragColor = texture(texture1, textureCoord) * uColor;

}