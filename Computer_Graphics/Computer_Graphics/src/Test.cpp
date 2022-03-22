#include "pch.h"
#include "Test.h"

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	//L_INFO("Attaching {0}...", this->m_debugName);

	m_ifsShader.Compile("Shader/IFS.vert", "Shader/IFS.frag");

	m_ifsShader.Use();
	rayquad.GenVertexObject();
}
void Test::OnDetach()
{
	//L_INFO("Detaching {0}...", this->m_debugName);
}

void Test::OnUpdate(vlr::Time time)
{	
	//hdrBuffer.BindBuffer();

	m_ifsShader.Use();
	m_ifsShader.SetInt("frame", time.frame_count);
	m_ifsShader.SetFloat("SystemTime", static_cast<float>(time.current_time));
	m_ifsShader.SetVec2("SystemResolution", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

	rayquad.Render();
}

void Test::ProcessInput()
{
    if (m_input.IsKeyPressed(VLR_KEY_PAGE_UP))
        m_exposure += 0.01f;
    if (m_input.IsKeyPressed(VLR_KEY_PAGE_DOWN))
        m_exposure -= 0.01f;

    if (m_exposure < 0)
        m_exposure = 0;
}

