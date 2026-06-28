#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D depthTex;

void main()
{             
    float depthValue = texture(depthTex, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}