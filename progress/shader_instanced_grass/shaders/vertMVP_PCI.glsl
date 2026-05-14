#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 positions[50]; //how do I make this [50] not a fixed number?

out vec3 Color;

void main()
{
    gl_Position = projection*view*model*vec4(aPos + positions[gl_InstanceID], 1.0);
    Color = aColor;
}