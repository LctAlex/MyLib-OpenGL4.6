#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
//???

out vec3 Color;

uniform mat4 lightSpaceMatrix; //projection + view of the LIGHT (for shadow)
out vec4 FragPosLightSpace;

void main()
{
    Color = aColor;
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}