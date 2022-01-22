#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

vec3 objectColor = vec3(0.0);

void main()
{      
    FragColor = vec4(objectColor, 1.0);
}