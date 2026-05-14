#version 460 core
out vec4 FragColor;
uniform float time;
uniform vec2 windowRes;

uniform vec3 camPos;

void main()
{
    vec2 uv = (gl_FragCoord.xy / windowRes) * 2.0 - 1.0; 

    //vec3 color = vec3(funcValue);
    // float d = length(uv);
    // vec3 color = vec3(smoothstep(0.5, 0.8, d));

    float d = length(vec2(sqrt(pow(camPos.x, 2) + pow(camPos.z, 2)), 0.0))/20.0;
    float fogColor = 0.5;
    vec3 color = vec3(min(fogColor, d-0.5f));

    FragColor = vec4(color, 1.0);
}

//fog effect
// float d = length(vec2(sqrt(pow(camPos.x, 2) + pow(camPos.z, 2)), 0.0))/10.0;
// vec3 color = vec3(uv, 0.5) +  d;