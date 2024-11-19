#version 330 core
out vec4 FragColor;

uniform vec4 debug_color;

void main()
{
    FragColor = debug_color;
}