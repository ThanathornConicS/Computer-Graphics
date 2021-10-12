#include "pch.h"
#include "Texture.h"

Texture::Texture(void)
    :m_textureID(0)
{}
Texture::Texture(const char* filepath, bool enableGamma)
    :m_textureID(0)
{
    LoadTexture(filepath, enableGamma);
}

uint32_t Texture::LoadTexture(const char* filepath, bool enableGamma)
{
    glGenTextures(1, &m_textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = enableGamma ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = enableGamma ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        L_SYSTEM_WARN("Texture failed to load at path: {0}", filepath);
        stbi_image_free(data);
    }
    return m_textureID;
}