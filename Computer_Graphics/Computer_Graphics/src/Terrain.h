#pragma once

#include "Scene.h"
#include "VLRCore.h"

class Terrain : public vlr::Scene
{
public:
	Terrain();
	virtual ~Terrain();

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

