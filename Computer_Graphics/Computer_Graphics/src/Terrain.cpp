#include "pch.h"
#include "Terrain.h"

#include "DiamonSquare.h"

vlr::Camera camera1(glm::vec3(0.0f, 0.0f, 7.0f));
int terrainGrid[64][64];

Terrain::Terrain()
	: Scene("Terrain"), m_terrainMesh(63, 63)
{}
Terrain::~Terrain()
{}

void Terrain::OnAttach()
{
	m_shader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

	for (size_t i = 0; i < 64; i++) 
	{
		for (size_t j = 0; j < 64; j++) 
		{
			terrainGrid[i][j] = 0;
		}
	}

	terrainGrid[0][0] = 7;
	terrainGrid[0][63] = 9;
	terrainGrid[63][63] = 4;
	terrainGrid[63][0] = 5;

	DiamonSquare(terrainGrid, 64);
	
	std::vector<float> heightData;
	for (size_t i = 0; i < 64; i++)
	{
		for (size_t j = 0; j < 64; j++)
		{
			heightData.push_back(terrainGrid[i][j]);
		}
	}

	m_terrainMesh.GenTriangleMesh(heightData);
	m_terrainMesh.GenTriangleNormals();

	m_terrainMesh.GenVertexObject();
}
void Terrain::OnDetach()
{}

void Terrain::OnUpdate(vlr::Time time)
{
	m_shader.Use();

	glm::mat4 projection = glm::perspective(glm::radians(camera1.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera1.GetViewMatrix(); // remove translation from the view matrix
	m_shader.SetMat4("view", view);
	m_shader.SetMat4("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	m_shader.SetMat4("model", model);

	m_terrainMesh.Render();
}

void Terrain::ProcessInput()
{
}
