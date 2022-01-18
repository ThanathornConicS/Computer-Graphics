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
vlr::Texture cubeMap;
SkyboxMesh skybox;
RayQuadMesh rayquad;

glm::mat4 shaderRotMat = glm::mat4(1.0f);
vlr::Camera camera(glm::vec3(0.0f, 1.0f, -7.0f));

float zoom = 1.0f;
int shapeSelected = 1;


RayMarchScene::RayMarchScene()
	: Scene("RayMarched")
{}
RayMarchScene::~RayMarchScene()
{}

void RayMarchScene::OnAttach()
{
    L_INFO("Attaching {0}...", this->m_debugName);

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

    printf("Controls:\n");
    printf("    1: Rhombic Dodecahedron\n");
    printf("    2: Cube\n");
    printf("    3: Triangle Prism\n");
    printf("    Scroll Wheel: Zoom In-Out\n");
    printf("    D: + Rotation Rate\n");
    printf("    A: - Rotation Rate\n");
    printf("    Pg Up: + Exposure\n");
    printf("    Pg Down: - Exposure\n");
}
void RayMarchScene::OnDetach()
{
    L_INFO("Detaching {0}...", this->m_debugName);
}
void RayMarchScene::OnUpdate(vlr::Time time)
{
    ProcessInput();

    shaderRotMat = glm::rotate(shaderRotMat, static_cast<float>(m_rotRate * time.deltaTime), glm::vec3(0, 1, 1));

    hdrBuffer.BindBuffer();

    m_rayMarchShader.Use();

    m_rayMarchShader.SetFloat("SystemTime", static_cast<float>(time.current_time));
    m_rayMarchShader.SetVec2("SystemResolution", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_rayMarchShader.SetFloat("zoom", zoom);
    m_rayMarchShader.SetMat4("rotMat", shaderRotMat);
    m_rayMarchShader.SetInt("shape", shapeSelected);

    rayquad.Render();

    // draw skybox as last
    m_skyBoxShader.Use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
    m_skyBoxShader.SetMat4("view", view);
    m_skyBoxShader.SetMat4("projection", projection);

    skybox.Render(&cubeMap);

    // Render Everything as hdr buffer
    m_hdrShader.Use();
    hdrBuffer.RenderBuffer();
    m_hdrShader.SetFloat("exposure", m_exposure);
    quadMesh.Render();
}

void RayMarchScene::ProcessInput()
{
    if (m_input.IsKeyPressed(VLR_KEY_1))
        shapeSelected = 1;
    else if (m_input.IsKeyPressed(VLR_KEY_2))
        shapeSelected = 2;
    else if (m_input.IsKeyPressed(VLR_KEY_3))
        shapeSelected = 3;

    if (m_input.IsKeyPressed(VLR_KEY_W))
        zoom += 0.1f;
    if (m_input.IsKeyPressed(VLR_KEY_S))
        zoom -= 0.1f;
    if (m_input.IsKeyPressed(VLR_KEY_D))
        m_rotRate += 0.01f;
    if (m_input.IsKeyPressed(VLR_KEY_A))
        m_rotRate -= 0.01f;
    if (m_input.IsKeyPressed(VLR_KEY_PAGE_UP))
        m_exposure += 0.01f;
    if (m_input.IsKeyPressed(VLR_KEY_PAGE_DOWN))
        m_exposure -= 0.01f;

    if (zoom < 0)
        zoom = 0;
    if (m_rotRate < 0)
        m_rotRate = 0;
    if (m_exposure < 0)
        m_exposure = 0;
}
