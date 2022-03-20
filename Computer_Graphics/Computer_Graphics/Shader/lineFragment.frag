#version 130

in vec3 Position;

uniform vec3 Kd; 	
uniform vec3 Ld; 		
uniform float Shininess;
uniform vec3 LightPosition; 		
uniform mat4 ViewMatrix;

out vec4 Color0;

void main( void )
{
	Color0 = vec4(0.6941, 0.0, 0.0, 1.0);
}
