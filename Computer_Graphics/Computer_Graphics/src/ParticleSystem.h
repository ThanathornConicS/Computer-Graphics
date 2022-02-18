#pragma once

#include "VLRCore.h"

namespace vlr 
{
	struct ParticleProps 
	{
		glm::vec3 Position;
		glm::vec3 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem 
	{
	public:
		ParticleSystem();

		void OnUpdate(Time time);
		void OnRender(Camera& camera);

		void Emit(const ParticleProps& particleProps);

	private:
		struct Particle 
		{
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		QuadMesh m_quad;
		GLuint m_QuadVA = 0;
		std::unique_ptr<Shader> m_ParticleShader;
		/*GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;*/

	};
}
