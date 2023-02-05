#include "pch.h"
#include "Test.h"

#include <thread>
#include "Camera.h"
#include "SphereMesh.h"

int arr[20] = {};
std::vector<int> vec;

vlr::Camera camera2(glm::vec3(0.0f, 0.0f, 7.0f));
SphereMesh sphere;

void foo(int startIdx, int amout, int value) 
{
	for (size_t i = startIdx; i < amout; i++) 
	{
		//arr[i] = value;
		vec.emplace_back(value);
		L_INFO("Foo: {0}, {1}", i, value);
	}
}
void bar(int startIdx, int amout, int value)
{
	for (size_t i = startIdx; i < amout; i++)
	{
		//arr[i] = value;
		vec.emplace_back(value);
		L_INFO("Bar: {0}, {1}", i, value);
	}
}

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	normalShader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");
	normalShader.Use();
}
void Test::OnDetach()
{}

void Test::OnUpdate(vlr::Time time)
{
	normalShader.Use();

	// Set a camera projection to shader
	glm::mat4 projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera2.GetViewMatrix())); // remove translation from the view matrix
	normalShader.SetVec3("viewPos", camera2.Position);
	normalShader.SetMat4("view", view);
	normalShader.SetMat4("projection", projection);

	// Set model matrix for object to render
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	normalShader.SetMat4("model", model);

	// Set Uniform for shader
	glm::vec3 lightColor = glm::vec3(0.5f, 0.2f, 0.7f);

	normalShader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.3f));
	normalShader.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.3f));
	normalShader.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	normalShader.SetFloat("material.shininess", 10.0f);

	normalShader.SetVec3("light.position", glm::vec3(1.0f, -1.0f, 0.0f));
	normalShader.SetVec3("light.ambient", lightColor * 0.7f);
	normalShader.SetVec3("light.diffuse", lightColor * 0.2f);
	normalShader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// Render object
	sphere.Render();
}
