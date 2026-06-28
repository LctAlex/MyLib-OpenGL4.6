#version 460 core
in vec3 Color;

out vec4 FragColor;

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
uniform float shadowDarkness = 0.7;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    //perspective division (for ortho/persp compatibility)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //converting from [-1,1] to [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0) return 0.0; //???
    
    float closestDepth = texture(shadowMap, projCoords.xy).r; //magic coords
    float currentDepth = projCoords.z;
    float bias = 0.0005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 result = Color *(1.0 - shadow * shadowDarkness);
    FragColor = vec4(result, 1.0);
}