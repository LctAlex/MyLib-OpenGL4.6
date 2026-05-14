#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 viewMat; //light's View matrix
uniform mat4 projMat; //light's Projection matrix
uniform mat4 model;

void main()
{
    gl_Position = projMat * viewMat * model * vec4(aPos, 1.0);
}