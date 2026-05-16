#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 4) in mat4 instanceModel;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model = mat4(1.0);

uniform vec3 positions[50]; //this is a mistake, what do these do here? I forgot about them

out vec3 Color;

void main()
{
    gl_Position = projection*view*model*instanceModel*vec4(aPos + positions[gl_InstanceID], 1.0);
    Color = aColor;
}