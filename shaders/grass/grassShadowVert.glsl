#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float swayForce;
layout (location = 4) in mat4 instanceModel;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model = mat4(1.0);

uniform float time;

uniform vec3 windDirection = vec3(0.3f, 0.f, 0.3f); //offset of how distanced tips are from the middle
uniform float windForce = 0.1f; //how much to move from the offset, will be used with sin(time)
uniform float windSpeed = 3.f;
 
vec3 calculateSway(vec3 windDirection, float windForce)
{
    return swayForce * vec3(windDirection.x + windForce * sin(time * windSpeed), 
                            0.f, 
                            windDirection.z + windForce  * cos(time * windSpeed));
}

out vec3 Color;

uniform mat4 lightSpaceMatrix; //projection + view of the LIGHT (for shadow)
out vec4 FragPosLightSpace;

void main()
{
    Color = aColor;// + (instanceModel*vec4(aPos, 1.0)).xyz;
    vec3 swayOffset = calculateSway(windDirection, windForce);
    FragPosLightSpace = lightSpaceMatrix * /*model **/instanceModel * vec4(aPos + swayOffset, 1.0);
    
    gl_Position = projection*view*/*model **/instanceModel*vec4(aPos + swayOffset, 1.0);
}