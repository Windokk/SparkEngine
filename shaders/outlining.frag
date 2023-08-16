#version 330 core


out vec4 FragColor;

vec4 outlineColor = vec4(0.8, 0.1090, 0.0,1.0);

void main()
{
    // If the color is yellow, set it to texColor2; otherwise, keep the original color
    FragColor = outlineColor;
}