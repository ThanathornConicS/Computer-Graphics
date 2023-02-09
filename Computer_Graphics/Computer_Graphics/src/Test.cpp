#include "pch.h"
#include "Test.h"

#include <thread>
#include "Camera.h"
#include "SphereMesh.h"
#include "CubeMesh.h"
#include "QuadMesh.h"

#include "imgui.h"

int arr[20] = {};
std::vector<int> vec;

vlr::Camera camera2(glm::vec3(0.0f, 0.0f, 0.0f));

SphereMesh sphere;
float spherePosition[3] = { 0.0f, 0.0f, -7.0f };
float sphereScale = 0.1f;
CubeMesh cube;
QuadMesh quad;

bool isDemoOpen = true;

Material material;
Light light;
bool setManualMaterial = false;
const char* materialTypeCombo[] = { "EMERALD", "OBSIDIAN", "GOLD", "CYANPLASTIC" };
static int currentMaterialType = 0;
const char* LightTypeCombo[] = { "POINT", "DIRECTIONAL", "SPOTLIGHT" };
static int currentLightType = 0;

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
	material = emeraldMaterial;

	m_hdrShader.Compile("Shader/hdrVertex.vert", "Shader/hdrFragment.frag");

	m_normalShader.Compile("Shader/normalVertexShader.vert", "Shader/normalFragmentShader.frag");
	m_normalShader.Use();

	m_hdrBuffer.CreateBuffer();
}
void Test::OnDetach()
{}

int recordIdx = 0;

void Test::OnUpdate(vlr::Time time)
{
	std::pair<int, int> screenSize = vlr::Window::GetInstance().GetWindowSize();

	m_hdrBuffer.BindBuffer();

	m_normalShader.Use();

	// Set a camera projection to shader
	glm::mat4 projection = glm::perspective(glm::radians(camera2.Zoom), (float)screenSize.first / (float)screenSize.second, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera2.GetViewMatrix())); // remove translation from the view matrix
	m_normalShader.SetVec3("viewPos", camera2.Position);
	m_normalShader.SetMat4("view", view);
	m_normalShader.SetMat4("projection", projection);

	{
		ImGui::ShowDemoWindow(&isDemoOpen);
		ImGui::Begin("Render Properties");

		ImGui::Text("Object Properties");
		ImGui::Separator();
		ImGui::SliderFloat3("Cube Position", spherePosition, -20.0f, 20.0f, "%.5f");
		ImGui::SliderFloat("Sphere Scale", &sphereScale, 0.0f, 1.0f, "%.5f");

		ImGui::Text("Object Material");
		ImGui::Separator();
		ImGui::Checkbox("Manual Material", &setManualMaterial);
		if (!setManualMaterial)
		{
			ImGui::Combo("Material Preset", &currentMaterialType, materialTypeCombo, IM_ARRAYSIZE(materialTypeCombo));
			switch (currentMaterialType)
			{
			case MaterialPreset::EMERALD:
				material = emeraldMaterial;
				break;
			case MaterialPreset::OBSIDIAN:
				material = obsidianMaterial;
				break;
			case MaterialPreset::GOLD:
				material = goldMaterial;
				break;
			case MaterialPreset::CYANPLASTIC:
				material = cyanPlasticMaterial;
				break;
			default:
				material = emeraldMaterial;
				break;
			}
		}
		ImGui::ColorEdit3("Ambient", material.ambient);
		ImGui::ColorEdit3("Diffuse", material.diffuse);
		ImGui::ColorEdit3("Specular", material.specular);
		ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 1.0f, "%.5f");
		ImGui::NewLine();

		ImGui::Text("Light Properties");
		ImGui::Separator();
		ImGui::Combo("Light Type", &currentLightType, LightTypeCombo, IM_ARRAYSIZE(LightTypeCombo));
		ImGui::SliderFloat3("Light Position", light.position, -30.0f, 30.0f);
		ImGui::SliderFloat3("Light Direction", light.direction, -1.0f, 1.0f);
		ImGui::ColorEdit3("Light Color", light.color);
		ImGui::SliderFloat("Light Ambient Strength", &light.ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Light Diffuse Strength", &light.diffuseStrength, 0.0f, 1.0f);
		ImGui::SliderFloat3("Light Specular", light.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Light Constant", &light.constant, 0.0f, 1.0f, "%.5f");
		ImGui::SliderFloat("Light Linear", &light.linear, 0.0f, 1.0f, "%.5f");
		ImGui::SliderFloat("Light Quadratic", &light.quadratic, 0.0f, 1.0f, "%.5f");
		ImGui::SliderAngle("Light Cut-Off", &light.cutOff, 0.0f, 360.0f, "%.3f");
		ImGui::SliderAngle("Light Outter Cut-Off", &light.outterCutOff, 0.0f, 360.0f, "%.3f");

		ImGui::Text("Scene Settings");
		ImGui::Separator();
		ImGui::SliderFloat("Exposure", &m_exposure, 0.0f, 1.0f);

		ImGui::End();
	}
	
	// Set Uniform for shader
	m_normalShader.SetVec3("material.ambient", material.ambient[0], material.ambient[1], material.ambient[2]);
	m_normalShader.SetVec3("material.diffuse", material.diffuse[0], material.diffuse[1], material.diffuse[2]);
	m_normalShader.SetVec3("material.specular", material.specular[0], material.specular[1], material.specular[2]);
	m_normalShader.SetFloat("material.shininess", material.shininess * 128.0f);
		
	m_normalShader.SetInt("lightType", currentLightType);
	m_normalShader.SetVec3("light.position", light.position[0], light.position[1], light.position[2]);
	m_normalShader.SetVec3("light.direction", light.direction[0], light.direction[1], light.direction[2]);
	m_normalShader.SetVec3("light.ambient", light.color[0] * light.ambientStrength, light.color[1] * light.ambientStrength, light.color[2] * light.ambientStrength);
	m_normalShader.SetVec3("light.diffuse", light.color[0] * light.diffuseStrength, light.color[1] * light.diffuseStrength, light.color[2] * light.diffuseStrength);
	m_normalShader.SetVec3("light.specular", light.specular[0], light.specular[1], light.specular[2]);
	m_normalShader.SetFloat("light.constant", light.constant);
	m_normalShader.SetFloat("light.linear", light.linear);
	m_normalShader.SetFloat("light.quadratic", light.quadratic);
	m_normalShader.SetFloat("light.cutOff", glm::cos(glm::radians(light.cutOff)));
	m_normalShader.SetFloat("light.outterCutOff", glm::cos(glm::radians(light.outterCutOff)));

	// Set model matrix for object to render
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(spherePosition[0], spherePosition[1], spherePosition[2]));
	model = glm::scale(model, glm::vec3(sphereScale));
	m_normalShader.SetMat4("model", model);

	// Render object
	sphere.Render();

	m_normalShader.SetVec3("material.ambient",glm::vec3(1.0f));
	m_normalShader.SetVec3("material.diffuse", glm::vec3(1.0f));
	m_normalShader.SetVec3("material.specular", glm::vec3(1.0f));
	m_normalShader.SetFloat("material.shininess", 32.0f);

	// Set model matrix for object to render
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(light.position[0], light.position[1], light.position[2]));
	model = glm::scale(model, glm::vec3(0.1f));
	m_normalShader.SetMat4("model", model);

	// Render object
	cube.Render();

	// Render Everything as hdr buffer
	m_hdrShader.Use();
	m_hdrBuffer.RenderBuffer();
	m_hdrShader.SetFloat("exposure", m_exposure);
	quad.Render();
}
