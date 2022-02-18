#version 450 core

layout (location = 0) in vec3 a_Position;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(a_Position, 1.0);
}