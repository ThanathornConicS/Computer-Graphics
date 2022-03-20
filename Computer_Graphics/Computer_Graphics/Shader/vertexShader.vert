#version 450 core

layout (location = 0) in vec3 a_Position;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(a_Position, 1.0);
}