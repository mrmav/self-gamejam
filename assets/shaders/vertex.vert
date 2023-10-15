#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexture;

out vec4 vertexColor;
out vec2 textureCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
    
    vertexColor = aColor;
    textureCoord = aTexture;
    
    // by flooring the screenpos, we get rid the tilemap lines
    // caused by the half pixel sampling.
    vec4 screenpos = uView * uModel * vec4(aPos, 1.0);
    vec4 pos = uProjection * vec4(floor(screenpos.xyz), screenpos.w);

    gl_Position = pos;

}