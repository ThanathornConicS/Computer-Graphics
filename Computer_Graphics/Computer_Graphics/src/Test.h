#pragma once

#include "Scene.h"
#include "VLRCore.h"

class Test : public vlr::Scene
{
public:
	Test();
	virtual ~Test();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(vlr::Time time) override;

	void ProcessInput();

private:
	vlr::Shader m_ifsShader;
	vlr::Shader m_hdrShader;

	RayQuadMesh rayquad;
	QuadMesh quadMesh;

	float m_exposure = 1.0f;

};


