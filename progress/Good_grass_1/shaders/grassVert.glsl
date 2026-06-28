#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float swayForce;
layout (location = 4) in mat4 instanceModel;

uniform float time;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model = mat4(1.0);

//uniform vec3 windDirection;
//uniform float windForce;

out vec3 Color;

void main()
{
    float len = instanceModel[1][1];
    vec3 swayOffset = swayForce * vec3(len*0.5 + sin(len) * sin(time*0.5), 0.0, len*0.5 + sin(len) * cos(time*0.5));//mamaliga
    gl_Position = projection*view*model*instanceModel*vec4(aPos + swayOffset, 1.0);
    Color = aColor;
}