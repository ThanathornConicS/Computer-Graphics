#include "pch.h"
#include "Test.h"

#include "Math.h"
#include "Physic.h"

QuadMesh quad;

//vlr::Camera camera0(glm::vec3(0.0f, 0.0f, 10.0f));

static double last_xpos = SCREEN_WIDTH / 2;
static double last_ypos = SCREEN_HEIGHT / 2;

float horizontalAngle = PI;
float verticalAngle = 0.0f;
float initialFoV = 45.0f;

glm::vec3 position = glm::vec3(0, 0, 6);
glm::vec3 direction = glm::vec3(0, 0, 0);
glm::vec3 right = glm::vec3(0, 0, 0);

float speed = 3.0f;
float mouseSensitivity = 0.0005f;

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	L_INFO("Attaching {0}...", this->m_debugName);

	m_shader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");
	m_shader.Use();

	quad.GenVertexObject();
}
void Test::OnDetach()
{
	L_INFO("Detaching {0}...", this->m_debugName);
}

void Test::OnUpdate(vlr::Time time)
{	
	double mouseX, mouseY;
	glfwGetCursorPos(vlr::Window::GetInstance().GetWindow(), &mouseX, &mouseY);

	if (m_input.IsMouseButtonPressed(VLR_MOUSE_BUTTON_LEFT)) 
	{
		horizontalAngle += mouseSensitivity * float(last_xpos - mouseX);
		verticalAngle += mouseSensitivity * float(last_ypos - mouseY);

		//L_TRACE("CLICKED");
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (m_input.IsKeyPressed(VLR_KEY_W)) {
		position += direction * static_cast<float>(time.deltaTime) * speed;
	}
	// Move backward
	if (m_input.IsKeyPressed(VLR_KEY_S)) {
		position -= direction * static_cast<float>(time.deltaTime) * speed;
	}
	// Strafe right
	if (m_input.IsKeyPressed(VLR_KEY_D)) {
		position += right * static_cast<float>(time.deltaTime) * speed;
	}
	// Strafe left
	if (m_input.IsKeyPressed(VLR_KEY_A)) {
		position -= right * static_cast<float>(time.deltaTime) * speed;
	}
	// Move Up
	if (m_input.IsKeyPressed(VLR_KEY_E)) {
		position += up * static_cast<float>(time.deltaTime) * speed;
	}
	// Move Down
	if (m_input.IsKeyPressed(VLR_KEY_Q)) {
		position -= up * static_cast<float>(time.deltaTime) * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, W:H ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection = glm::perspective(FoV, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	// Camera matrix
	glm::mat4 view = glm::lookAt(
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
	);

	last_xpos = xpos;
	last_ypos = ypos;

	glm::mat4 model = glm::mat4(1.0f);
	/*glm::mat4 view = m_camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);*/

	m_shader.Use();
	m_shader.SetMat4("view", view);
	m_shader.SetMat4("projection", projection);

	// Floor
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
	m_shader.SetMat4("model", model);
	quad.Render();
}

