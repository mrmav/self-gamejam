#version 460 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform float uTime;
uniform sampler2D texture1;

void main(){

    FragColor = texture(texture1, textureCoord) * vertexColor;

}