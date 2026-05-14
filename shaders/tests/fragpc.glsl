#version 460 core
out vec4 FragColor;
in vec3 PosCol;

void main()
{
    FragColor = vec4(PosCol + 0.5, 1.0);
}