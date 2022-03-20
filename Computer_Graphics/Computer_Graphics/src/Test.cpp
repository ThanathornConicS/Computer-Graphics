#include "pch.h"
#include "Test.h"

vlr::Camera camera0(glm::vec3(0.0f, 0.0f, 1.0f));

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	//L_INFO("Attaching {0}...", this->m_debugName);

	m_hdrShader.Compile("Shader/hdrVertex.vert", "Shader/hdrFragment.frag");
	m_ifsShader.Compile("Shader/IFS.vert", "Shader/IFS.frag");

	hdrBuffer.CreateBuffer();
	quadMesh.GenVertexObject();

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

	// Render Everything as hdr buffer
	/*m_hdrShader.Use();
	hdrBuffer.RenderBuffer();
	m_hdrShader.SetFloat("exposure", m_exposure);
	quadMesh.Render();*/
}

