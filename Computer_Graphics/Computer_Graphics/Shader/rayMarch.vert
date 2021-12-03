#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec3 fragCoord;
out vec2 texCoord;

void main()
{   
    fragCoord = aPos;
    texCoord = aTexCoord;
    gl_Position = vec4(fragCoord, 1);
}