#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float swayForce;
layout (location = 4) in mat4 instanceModel;

// uniform mat4 projection;
// uniform mat4 view;
uniform mat4 lightSpaceMatrix;
uniform mat4 model = mat4(1.0);

uniform float time;

uniform vec3 windDirection = vec3(0.3f, 0.f, 0.3f); //offset of how distanced tips are from the middle
uniform float windForce = 0.1f; //how much to move from the offset, will be used with sin(time)
uniform float windSpeed = 3.f;

vec3 calculateSway(vec3 windDirection, float windForce, float random)
{
    return swayForce * vec3(windDirection.x + windForce * random * sin(time * windSpeed), 
                            0.f, 
                            windDirection.z + windForce  * random * cos(time * windSpeed));
}

void main()
{
    float len = instanceModel[1][1];
    float randomFactor = len;
    vec3 swayOffset = calculateSway(windDirection, windForce, randomFactor);
    // gl_Position = projection*view*model*instanceModel*vec4(aPos + swayOffset, 1.0);
    gl_Position = lightSpaceMatrix*model*instanceModel*vec4(aPos + swayOffset, 1.0);
}