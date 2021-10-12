#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <pch.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GL/glew.h"

class Texture
{
public:
	Texture(void);
	Texture(const char* filepath, bool enableGamma);

	uint32_t LoadTexture(const char* filepath, bool enableGamma);

private:
	uint32_t m_textureID;

};

#endif // !TEXTURELOADER_H


