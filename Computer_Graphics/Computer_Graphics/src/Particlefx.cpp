#include "pch.h"
#include "Particlefx.h"

const int N = 20;

vlr::Camera camera0(glm::vec3(0.0f, 0.0f, 7.0f));
std::vector<glm::vec3> results;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

Particlefx::Particlefx()
	:Scene("ParticleTesting")
{}
Particlefx::~Particlefx()
{}

void Particlefx::OnAttach()
{
	m_shader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

	results.resize(N);

	for (size_t i = 0; i < N; i++) 
	{
		double theta = 2.0 * static_cast<double>(PI) * static_cast<double>(vlr::Random::Float());
		double phi = acos(1 - 2 * vlr::Random::Float());

		results[i] = glm::vec3(sin(phi) * cos(theta), sin(phi) * cos(theta), cos(phi));
	}

	for (auto& points : results) 
	{
		L_TRACE("Points: {0}, {1}, {2}", points.x, points.y, points.z);
	}

	m_shader.Use();
	cubeMesh.GenVertexObject();
	quadMesh.GenVertexObject();
}
void Particlefx::OnDetach()
{}

void Particlefx::OnUpdate(vlr::Time time)
{
	m_shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera0.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera0.GetViewMatrix(); // remove translation from the view matrix
	m_shader.SetMat4("view", view);
	m_shader.SetMat4("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	m_shader.SetMat4("model", model);

	cubeMesh.Render();

	/*for (size_t i = 0; i < N; i++) 
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, results[i]);
		model = glm::scale(model, glm::vec3(0.5f));
		m_shader.SetMat4("model", model);

		cubeMesh.Render();
	}*/

	glm::vec2 mousePos = m_input.GetMousePosition();
	//L_TRACE("Mouse: {0}, {1}", mousePos.x, mousePos.y);

	if (firstMouse)
	{
		lastX = mousePos.x;
		lastY = mousePos.y;
		firstMouse = false;
	}

	float xoffset = mousePos.x - lastX;
	float yoffset = lastY - mousePos.y; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	camera0.ProcessMouseMovement(xoffset, yoffset);
}
