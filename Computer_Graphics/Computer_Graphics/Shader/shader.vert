#version 450

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 texCoord;

void main()
{
    fragColor = aColor;
    texCoord = aTexCoord;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(aPos, 1.0);
}