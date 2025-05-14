#pragma once

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
		
		void ImportModel(const char* filepath);
		void _ImportModel(std::string& filepath);

		void GenerateVAO();
		void Render();

		inline bool IsMeshQuad() const { return isQuad; }

	public:
		uint32_t modelVAO = 0;
		uint32_t modelVBO = 0;

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec2> m_uv;
		std::vector<glm::vec3> m_normals;

		std::vector<float> model_vertices;

		bool hasVertices = false;
		bool hasNormals = false;
		bool hasUVs = false;

		bool isQuad = false;
	};
}

