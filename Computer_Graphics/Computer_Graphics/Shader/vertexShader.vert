#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{   
    // vec4 modPos = model * vec4(aPos, 1.0);
    // FragPos = modPos.xyz;
    Normal = aNormal;  
    
    //gl_Position = /*vec4(FragPos, 1.0);*/ projection * view * vec4(FragPos, 1.0);
    FragPos = (model * vec4(aPos, 1.0)).xyz;
    gl_Position = model * vec4(aPos, 1.0);
}