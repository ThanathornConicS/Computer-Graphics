#include "pch.h"
#include "Test.h"

#include <thread>
#include "Camera.h"
#include "SphereMesh.h"

int arr[20] = {};
std::vector<int> vec;

vlr::Camera camera2(glm::vec3(0.0f, 1.0f, 7.0f));
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
	/*std::thread fooThread(foo, 0, 10, 1);
	std::thread barThread(bar, 10, 20, 0);

	fooThread.join();
	barThread.join();

	for (size_t i = 0; i < 20; i++) 
	{
		L_TRACE("Arr[{0}] = {1}", i, vec[i]);
	}*/

	normalShader.Compile("Shader/vertexShader.vert", "Shader/fragmentShader.frag");
	normalShader.Use();
}
void Test::OnDetach()
{}

void Test::OnUpdate(vlr::Time time)
{
	normalShader.Use();

	glm::mat4 projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera2.GetViewMatrix())); // remove translation from the view matrix
	normalShader.SetMat4("view", view);
	normalShader.SetMat4("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f));
	normalShader.SetMat4("model", model);

	sphere.Render();
}
