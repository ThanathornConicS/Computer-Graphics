#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <pch.h>
#include "GL/glew.h"
#include "glm.hpp"

namespace vlr
{
	class Model
	{
	public:
		Model(void);
		Model(const char* filepath);
		~Model();

		void ImportModel(const char* filepath);
		void Render();

		uint32_t modelVAO = 0;
		uint32_t modelVBO = 0;

	private:

		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec2> m_uv;
		std::vector<glm::vec3> m_normals;

		std::vector<float> model_vertices;

	};
}

#endif // !MODELLOADER_H


