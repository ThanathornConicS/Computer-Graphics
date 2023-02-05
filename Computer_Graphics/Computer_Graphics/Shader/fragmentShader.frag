#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material
{   
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{      
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
  	 
    // Diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflctDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflctDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    //FragColor = texture(texture_diffuse1, fs_in.TexCoords) * vec4(result, 1.0);
    FragColor = vec4(result, 1.0);
}