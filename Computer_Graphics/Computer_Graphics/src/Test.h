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

private:
	vlr::Shader m_ifsShader;
	vlr::Shader m_hdrShader;

	HDRBuffer hdrBuffer;
	RayQuadMesh rayquad;
	QuadMesh quadMesh;

	float m_exposure = 1.0f;

};


