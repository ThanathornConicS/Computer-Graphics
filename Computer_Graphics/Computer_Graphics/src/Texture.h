#pragma once

#include <pch.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GL/glew.h"

namespace vlr
{
	class Texture
	{
	public:
		Texture(void);
		Texture(const char* filepath, bool enableGamma);
		Texture(std::vector<std::string>& faces);

		uint32_t LoadTexture(const char* filepath, bool enableGamma);
		uint32_t LoadCubeMap(std::vector<std::string>& faces);

		inline uint32_t GetTextureID() const
		{
			return m_textureID;
		}

	private:
		uint32_t m_textureID;

	};
}


