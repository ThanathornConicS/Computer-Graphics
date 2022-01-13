#include "pch.h"
#include "RayMarchScene.h"

#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "Camera.h"

#include "RayQuadMesh.h"
#include "SkyboxMesh.h"
#include "QuadMesh.h"
#include "HDRBuffer.h"

#include "gtc/matrix_transform.hpp"

HDRBuffer hdrBuffer;
QuadMesh quadMesh;

Texture cubeMap;

SkyboxMesh skybox;

RayQuadMesh rayquad;

glm::mat4 shaderRotMat = glm::mat4(1.0f);

Camera camera(glm::vec3(0.0f, 1.0f, -7.0f));

float zoom = 1.0f;


RayMarchScene::RayMarchScene()
	: Scene("RayMarched")
{}
RayMarchScene::~RayMarchScene()
{}

void RayMarchScene::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset, zoom);
}

void RayMarchScene::OnAttach()
{
    m_hdrShader.Compile("Shader/hdrVertex.vert", "Shader/hdrFragment.frag");
    m_rayMarchShader.Compile("Shader/rayMarch.vert", "Shader/rayMarch.frag");
    m_skyBoxShader.Compile("Shader/skyBox.vert", "Shader/skyBox.frag");

    std::vector<std::string> faces
    {
        "Assets/CubeMaps/posx.jpg",
        "Assets/CubeMaps/negx.jpg",
        "Assets/CubeMaps/posy.jpg",
        "Assets/CubeMaps/negy.jpg",
        "Assets/CubeMaps/posz.jpg",
        "Assets/CubeMaps/negz.jpg"
    };
    cubeMap.LoadCubeMap(faces);

    hdrBuffer.CreateBuffer();
    quadMesh.GenVertexObject();

    m_skyBoxShader.Use();
    m_skyBoxShader.SetInt("skybox", 0);
    skybox.GenVertexObject();

    m_rayMarchShader.Use();
    rayquad.GenVertexObject();
}
void RayMarchScene::OnDeAttach()
{

}
void RayMarchScene::OnUpdate(GLFWwindow* window, Time time)
{
    glfwSetScrollCallback(window, MouseScrollCallback);
    ProcessInput(window);

    shaderRotMat = glm::rotate(shaderRotMat, (float)(m_rotRate * time.deltaTime), glm::vec3(0, 1, 1));

    hdrBuffer.BindBuffer();

    m_rayMarchShader.Use();

    m_rayMarchShader.SetFloat("SystemTime", time.current_time);
    m_rayMarchShader.SetVec2("SystemResolution", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_rayMarchShader.SetFloat("zoom", zoom);
    m_rayMarchShader.SetMat4("rotMat", shaderRotMat);

    rayquad.Render();

    // draw skybox as last
    m_skyBoxShader.Use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
    m_skyBoxShader.SetMat4("view", view);
    m_skyBoxShader.SetMat4("projection", projection);

    skybox.Render(&cubeMap);

    // Render Everything as hdr buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_hdrShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrBuffer.m_colorBuffer);
    m_hdrShader.SetFloat("exposure", m_exposure);
    quadMesh.Render();
}

void RayMarchScene::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_rotRate += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_rotRate -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        m_exposure += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        m_exposure -= 0.01f;
}
