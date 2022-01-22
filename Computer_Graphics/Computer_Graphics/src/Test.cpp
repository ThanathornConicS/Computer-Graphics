#include "pch.h"
#include "Test.h"

#include "Camera.h"
#include "CubeMesh.h"

CubeMesh cube;

glm::mat4 model = glm::mat4(1.0f);
vlr::Camera camera0(glm::vec3(0.0f, 0.0f, 3.0f));

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	L_INFO("Attaching {0}...", this->m_debugName);

	m_shader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

	cube.GenVertexObject();
}
void Test::OnDetach()
{
	L_INFO("Detaching {0}...", this->m_debugName);
}

void Test::OnUpdate(vlr::Time time)
{	
	glm::vec2 mousePos = m_input.GetMousePosition();
	L_INFO("X: {0}	Y: {1}", mousePos.x, mousePos.y);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.2));
	model = glm::rotate(model, 60.0f, glm::vec3(0, 1, 1));

	m_shader.Use();

	glm::mat4 projection = glm::perspective(glm::radians(camera0.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	m_shader.SetMat4("projection", projection);
	glm::mat4 view = glm::mat4(glm::mat3(camera0.GetViewMatrix())); // remove translation from the view matrix
	m_shader.SetMat4("view", view);

	m_shader.SetMat4("model", model);
	m_shader.SetBool("inverse_normals", false);

	cube.Render();
}
