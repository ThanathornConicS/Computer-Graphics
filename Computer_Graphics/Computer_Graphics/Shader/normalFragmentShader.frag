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
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outterCutOff;
};

uniform sampler2D texture_diffuse1;
uniform Material material;
uniform Light light;

uniform vec3 viewPos;
uniform int lightType;

float gamma = 2.2;

void main()
{      
    vec3 lightDir;
    float attenuation;
    if(lightType == 0 || lightType == 2)
    {
        lightDir = normalize(light.position - fs_in.FragPos);
        //lightDir = normalize(fs_in.FragPos - light.position);
        float distance = length(light.position - fs_in.FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    }
    else if(lightType == 1)
    {
       lightDir = normalize(-light.direction);
    }

    if(lightType == 2)
    {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOff - light.outterCutOff;
        float intensity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);
        if(theta > light.cutOff)
        {
            // Ambient
            vec3 ambient = light.ambient * material.ambient; 
     
            // Diffuse 
            vec3 norm = normalize(fs_in.Normal);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light.diffuse * (diff * material.diffuse);

            // Specular
            vec3 viewDir = normalize(viewPos - fs_in.FragPos);
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
            vec3 specular = light.specular * (spec * material.specular) * vec3(0.3);

            diffuse *= intensity;
            specular *= intensity;

            vec3 result = ambient + diffuse + specular;
            //FragColor = texture(texture_diffuse1, fs_in.TexCoords) * vec4(result, 1.0);
            FragColor = vec4(result, 1.0);
        }
        else
        {
            FragColor = vec4(light.ambient * material.diffuse, 1.0);
        }
    }
    else
    {
        // Ambient
        vec3 ambient = light.ambient * material.ambient; 
     
        // Diffuse 
        vec3 norm = normalize(fs_in.Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);

        // Specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular) * vec3(0.3);

        if(lightType == 0)
        {
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
        }

        vec3 result = ambient + diffuse + specular;
        //FragColor = texture(texture_diffuse1, fs_in.TexCoords) * vec4(result, 1.0);
        FragColor = vec4(result, 1.0);
    }
   
}