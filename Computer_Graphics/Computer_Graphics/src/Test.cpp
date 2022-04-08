#include "pch.h"
#include "Test.h"

#include "Bellman.h"

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	m_ifsShader.Compile("Shader/IFS.vert", "Shader/IFS.frag");

	m_ifsShader.Use();
	rayquad.GenVertexObject();

	Graph* graph = CreateGraph(10);

	graph->m_edge[0].m_src = 0;
	graph->m_edge[0].m_dest = 1;
	graph->m_edge[0].m_weight = 6;

	graph->m_edge[1].m_src = 0;
	graph->m_edge[1].m_dest = 3;
	graph->m_edge[1].m_weight = 7;

	graph->m_edge[2].m_src = 1;
	graph->m_edge[2].m_dest = 2;
	graph->m_edge[2].m_weight = 5;

	graph->m_edge[3].m_src = 1;
	graph->m_edge[3].m_dest = 3;
	graph->m_edge[3].m_weight = 8;

	graph->m_edge[4].m_src = 2;
	graph->m_edge[4].m_dest = 1;
	graph->m_edge[4].m_weight = -2;

	graph->m_edge[5].m_src = 3;
	graph->m_edge[5].m_dest = 2;
	graph->m_edge[5].m_weight = -3;

	graph->m_edge[6].m_src = 3;
	graph->m_edge[6].m_dest = 4;
	graph->m_edge[6].m_weight = 9;

	graph->m_edge[7].m_src = 4;
	graph->m_edge[7].m_dest = 1;
	graph->m_edge[7].m_weight = -4;

	graph->m_edge[8].m_src = 4;
	graph->m_edge[8].m_dest = 2;
	graph->m_edge[8].m_weight = 7;

	graph->m_edge[9].m_src = 4;
	graph->m_edge[9].m_dest = 0;
	graph->m_edge[9].m_weight = 2;

	//// add edge 0 - 1 (or A - B in above figure)
	//graph->m_edge[0].m_src = 0;
	//graph->m_edge[0].m_dest = 1;
	//graph->m_edge[0].m_weight = -1;

	//// add edge 0-2 (or A-C in above figure)
	//graph->m_edge[1].m_src = 0;
	//graph->m_edge[1].m_dest = 2;
	//graph->m_edge[1].m_weight = 4;

	//// add edge 1-2 (or B-C in above figure)
	//graph->m_edge[2].m_src = 1;
	//graph->m_edge[2].m_dest = 2;
	//graph->m_edge[2].m_weight = 3;

	//// add edge 1-3 (or B-D in above figure)
	//graph->m_edge[3].m_src = 1;
	//graph->m_edge[3].m_dest = 3;
	//graph->m_edge[3].m_weight = 2;

	//// add edge 1-4 (or B-E in above figure)
	//graph->m_edge[4].m_src = 1;
	//graph->m_edge[4].m_dest = 4;
	//graph->m_edge[4].m_weight = 2;

	//// add edge 3-2 (or D-C in above figure)
	//graph->m_edge[5].m_src = 3;
	//graph->m_edge[5].m_dest = 2;
	//graph->m_edge[5].m_weight = 5;

	//// add edge 3-1 (or D-B in above figure)
	//graph->m_edge[6].m_src = 3;
	//graph->m_edge[6].m_dest = 1;
	//graph->m_edge[6].m_weight = 1;

	//// add edge 4-3 (or E-D in above figure)
	//graph->m_edge[7].m_src = 4;
	//graph->m_edge[7].m_dest = 3;
	//graph->m_edge[7].m_weight = -3;
	
	BellmenFord(graph, 0);
}
void Test::OnDetach()
{}

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

