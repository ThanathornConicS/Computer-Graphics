#pragma once

#include <pch.h>
#include "GL/glew.h"
#include "glm.hpp"

struct TerrainMesh 
{
	TerrainMesh(int mapSize, int meshSize) 
		: m_mapSize(mapSize), m_meshSize(meshSize)
	{}

	std::vector<glm::vec3> m_triNormals;
	std::vector<float> m_mesh, m_normals;

	uint32_t terrainVBO, m_normalBuffer, terrainVAO;

	int m_sideNum;
	int m_mapSize;

	float m_meshSize;

	inline void GenTriangleMesh(std::vector<float> heightData) 
	{
		if (heightData.empty()) 
		{
			L_SYSTEM_WARN("Error: Height data is empty");
			return;
		}

		int sideNum = m_mapSize;

		float xspot = -1;
		float zspot = -1;
		float squaresize = m_meshSize / sideNum;

		for (int z = 0; z < sideNum; z++)
		{
			for (int x = 0; x < sideNum; x++)
			{
				m_mesh.insert(m_mesh.end(), { xspot, 0.0f, zspot });
				m_mesh.insert(m_mesh.end(), { xspot, 0.0f, zspot + squaresize });
				m_mesh.insert(m_mesh.end(), { xspot + squaresize, 0.0f, zspot });

				xspot += squaresize;
			}
			xspot = -1;
			for (int x = 0; x < sideNum; x++)
			{
				m_mesh.insert(m_mesh.end(), { xspot + squaresize, 0.0f, zspot + squaresize });
				m_mesh.insert(m_mesh.end(), { xspot + squaresize, 0.0f, zspot });
				m_mesh.insert(m_mesh.end(), { xspot, 0.0f, zspot + squaresize });

				xspot += squaresize;
			}
			xspot = -1;
			zspot += squaresize;
		}

		for (int i = 1; i < m_mesh.size(); i += 3)
		{
			m_mesh[i] = heightData[(m_mesh[i - 1] + 1) / squaresize + (sideNum + 1) * (m_mesh[i + 1] + 1) / squaresize];
		}

	}
	inline void GenTriangleNormals() 
	{
		for (int i = 0; i != m_mesh.size(); i += 9)
		{
			glm::vec3 first(m_mesh[i + 3] - m_mesh[i], m_mesh[i + 4] - m_mesh[i + 1], m_mesh[i + 5] - m_mesh[i + 2]);
			glm::vec3 second(m_mesh[i + 6] - m_mesh[i], m_mesh[i + 7] - m_mesh[i + 1], m_mesh[i + 8] - m_mesh[i + 2]);

			glm::vec3 normal = glm::cross(first, second);
			normal = glm::normalize(normal);

			m_triNormals.push_back(normal);
			m_triNormals.push_back(normal);
			m_triNormals.push_back(normal);
		}

		m_normals.resize(m_mesh.size()); //check

		for (int i = 0; i != m_triNormals.size(); i++)
		{
			m_normals[i * 3] = m_triNormals[i].x;
			m_normals[i * 3 + 1] = m_triNormals[i].y;
			m_normals[i * 3 + 2] = m_triNormals[i].z;
		}
	}

	inline void GenVertexObject() 
	{
		if (terrainVAO == 0) 
		{
			//generate vao
			glGenVertexArrays(1, &terrainVAO);
			glBindVertexArray(terrainVAO);

			glGenBuffers(1, &terrainVBO);
			glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
			glBufferData(GL_ARRAY_BUFFER, m_mesh.size() * sizeof(GLfloat), &(m_mesh[0]), GL_STATIC_DRAW);

			//buffer attribute for mesh
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glGenBuffers(1, &m_normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
			glBufferData(GL_ARRAY_BUFFER, m_mesh.size() * sizeof(GLfloat), &m_normals[0], GL_STATIC_DRAW);

			//bufffer for vertexnorms
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
	}

	inline void Render() 
	{
		GenVertexObject();
		glBindVertexArray(terrainVAO);
		glDrawArrays(GL_TRIANGLES, 0, m_mesh.size());
	}
};
