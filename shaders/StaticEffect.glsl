#version 460 core
out vec4 FragColor;
uniform float time;
uniform vec2 windowRes;

void main()
{
    vec2 uv = (gl_FragCoord.xy / windowRes) * 2.0 - 1.0; 
    //vec3 color = sin(vec3(10/uv.x-time*10)) * sin(vec3(10/uv.y+time*10));
    //vec3 color = sin(vec3(10/uv.y+time*10)); //* cos(vec3(100/uv.x+time*10));
    // vec3 color = sin(vec3(10/uv.y*uv.x+time*10));
    // vec3 color = sin(vec3(100/sin(uv.y*uv.x)*time));
    // vec3 color = sin(vec3(100/sin(cos(uv.y)*cos(uv.x))*time));
    // vec3 color = sin(100*time*vec3(uv.x / uv.y));
    // vec3 color = sin(100*time+vec3(uv.x / uv.y));
    //vec3 color = sin(vec3(uv.y+100/uv.x)* time);
    //vec3 color = vec3(abs(sin(uv.y)*cos(uv.x))); //nothing special (IT IS!)
    //vec3 color = (abs(uv.x) < 0.5 && abs(uv.y) < 0.5) ? vec3(1.0) : vec3(0.0);
    //vec3 color = vec3(abs(mod(uv.x,uv.y)));

    vec3 color = vec3(1.0);
    

    FragColor = vec4(color, 1.0);
}

//fence shader (on nothing)
// vec3 color = vec3(abs(sin(uv.y * 50)+cos(uv.x * 50))); //fence
// if(color.x > 0.9 && color.y > 0.9 && color.z > 0.9) color = vec3(0.0);