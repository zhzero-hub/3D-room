#version 330 core
out vec4 FragColor;

uniform bool Switch;

void main()
{
    if(Switch)
        FragColor = vec4(0.1); // set alle 4 vector values to 1.0
    else
        FragColor = vec4(1.0);
}