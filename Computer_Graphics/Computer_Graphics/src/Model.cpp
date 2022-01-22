#include "pch.h"
#include "Model.h"

namespace vlr
{
	Model::Model(void)
	{}
	Model::Model(const char* filepath)
	{
		ImportModel(filepath);
	}

	Model::~Model()
	{
		m_vertices.clear();
		m_uv.clear();
		m_normals.clear();

		model_vertices.clear();
	}

	void Model::ImportModel(const char* filepath)
	{
		L_SYSTEM_TRACE("Model: Loading {0}", filepath);

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;


		FILE* file = fopen(filepath, "r");
		if (file == NULL)
		{
			L_SYSTEM_WARN("Model: Cannot open {0}", filepath);
			return;
		}

		while (true)
		{

			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					L_SYSTEM_WARN("Model: System does not support this file format for {0}", filepath);
					fclose(file);
					return;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else
			{
				// Probably a comment, eat up the rest of the line
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}

		}

		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			model_vertices.push_back(vertex.x); model_vertices.push_back(vertex.y); model_vertices.push_back(vertex.z);
			model_vertices.push_back(uv.x); model_vertices.push_back(uv.y);
			model_vertices.push_back(normal.x); model_vertices.push_back(normal.y); model_vertices.push_back(normal.z);
		}

		fclose(file);

		temp_vertices.clear();
		temp_uvs.clear();
		temp_normals.clear();
	}

	void Model::Render()
	{
		if (modelVAO == 0)
		{
			glGenVertexArrays(1, &modelVAO);
			glGenBuffers(1, &modelVBO);
			glBindVertexArray(modelVAO);
			glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
			glBufferData(GL_ARRAY_BUFFER, model_vertices.size() * sizeof(float), &model_vertices[0], GL_STATIC_DRAW);
			GLsizei stride = (3 + 2 + 3) * sizeof(float);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
		}
		glBindVertexArray(modelVAO);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model_vertices.size()));
	}
}
