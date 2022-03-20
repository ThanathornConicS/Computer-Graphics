#include "pch.h"
#include "Particlefx.h"

const int N = 1000;

Particlefx::Particlefx()
	:Scene("ParticleTesting")
{}
Particlefx::~Particlefx()
{}

void Particlefx::OnAttach()
{
	vlr::Random::Init();
	
	std::vector<glm::vec3> results;
	results.resize(N);

	for (size_t i = 0; i < N; i++) 
	{
		double theta = 2.0 * static_cast<double>(PI) * static_cast<double>(vlr::Random::Float());
		double phi = acos(1 - 2 * vlr::Random::Float());

		results[i] = glm::vec3(sin(phi) * cos(theta), sin(phi) * cos(theta), cos(phi));
	}

	for (auto& points : results) 
	{
		L_TRACE("Points: {0}, {1}, {2}", points.x, points.y, points.z);
	}
}
void Particlefx::OnDetach()
{}

void Particlefx::OnUpdate(vlr::Time time)
{
}
