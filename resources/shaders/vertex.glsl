#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 model; 
uniform mat4 projection; 

out vec2 uv;
out mat4 Projection;

void main()
{
    Projection = projection;
    gl_Position = projection * model * vec4(vertex.xy, 0.0f, 1.0f);
    uv = vertex.zw;
}