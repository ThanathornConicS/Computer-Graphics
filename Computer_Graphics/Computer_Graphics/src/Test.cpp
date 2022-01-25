#include "pch.h"
#include "Test.h"

#include "Physic.h"

CubeMesh cube[10];
vlr::Shader shader;

vlr::Camera camera0(glm::vec3(0.0f, 0.0f, 10.0f));

glm::vec3 cubePositions[] = 
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	L_INFO("Attaching {0}...", this->m_debugName);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

	for (int i = 0; i < 10; i++)
		cube[i].GenVertexObject();

	DiiferentialEquation();
	printf("\n");
	EulerIntegration();

	//// Init here
	/*m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };*/
}
void Test::OnDetach()
{
	L_INFO("Detaching {0}...", this->m_debugName);
}

void Test::OnUpdate(vlr::Time time)
{	
	shader.Use();

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera0.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	shader.SetMat4("proj", projection);

	// camera/view transformation
	glm::mat4 view = camera0.GetViewMatrix();
	shader.SetMat4("view", view);

	shader.SetVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	for (size_t i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.SetMat4("model", model);

		cube[i].Render();
	}

	//if (m_input.IsMouseButtonPressed(VLR_MOUSE_BUTTON_LEFT)) 
	//{
	//	//L_TRACE("Clicked");

	//	glm::vec2 mousePos = m_input.GetMousePosition();
	//	glm::vec3 camPos = m_camera.Position;

	//	mousePos.x = (mousePos.x / SCREEN_WIDTH) * SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
	//	mousePos.y = SCREEN_HEIGHT * 0.5f - (mousePos.y / SCREEN_HEIGHT) * SCREEN_HEIGHT;

	//	m_particle.Position = { mousePos.x + camPos.x, mousePos.y + camPos.y };
	//	for (int i = 0; i < 5; i++)
	//		m_particleSystem.Emit(m_particle);
	//}

	//m_particleSystem.OnUpdate(time);
	//m_particleSystem.OnRender(m_camera);

}
