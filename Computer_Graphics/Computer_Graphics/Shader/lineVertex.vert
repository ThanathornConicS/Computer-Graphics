#version 130 

in vec3 aPos;
		
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

out vec3 Position;

void main( void )
{
	Position = vec3(ModelViewMatrix * vec4(aPos, 1.0));

	gl_Position = MVP * vec4(aPos, 1.0);
}