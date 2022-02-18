#include <pch.h>
#include "ParticleSystem.h"

#include <gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/compatibility.hpp>

#include <gtc/type_ptr.hpp>

namespace vlr
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
	}

	void ParticleSystem::OnUpdate(Time time)
	{
		for (auto& particle : m_ParticlePool) 
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f) 
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= time.deltaTime;
			particle.Position += particle.Velocity * (float)(time.deltaTime);
			particle.Rotation += 0.01f * time.deltaTime;

			//L_TRACE("LifeRemaining: {0}", particle.LifeRemaining);
		}
	}
	void ParticleSystem::OnRender(Camera& camera)
	{
		/*if (!m_QuadVA)
		{
			float vertices[] = {
				 -0.5f, -0.5f, 0.0f,
				  0.5f, -0.5f, 0.0f,
				  0.5f,  0.5f, 0.0f,
				 -0.5f,  0.5f, 0.0f
			};

			glCreateVertexArrays(1, &m_QuadVA);
			glBindVertexArray(m_QuadVA);

			GLuint quadVB, quadIB;
			glCreateBuffers(1, &quadVB);
			glBindBuffer(GL_ARRAY_BUFFER, quadVB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexArrayAttrib(quadVB, 0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

			uint32_t indices[] =
			{
				0, 1, 2, 2, 3, 0
			};

			glCreateBuffers(1, &quadIB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			m_ParticleShader = std::unique_ptr<Shader>(Shader::CompileFromText("Shader/particles.vert", "Shader/particles.frag"));
			m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->ID, "view");
			m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->ID, "model");
			m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->ID, "color");
		}

		m_ParticleShader->Use();
		glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));*/
		if (m_quad.quadVAO == 0) 
		{
			m_ParticleShader = std::unique_ptr<Shader>(Shader::CompileFromText("Shader/particles.vert", "Shader/particles.frag"));
			m_quad.GenVertexObject();
		}

		m_ParticleShader->Use();
		m_ParticleShader->SetMat4("view", camera.GetViewMatrix());
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_ParticleShader->SetMat4("proj", projection);

		for (auto& particle : m_ParticlePool) 
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

			/*glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
			glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
			glBindVertexArray(m_QuadVA);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);*/

			m_ParticleShader->SetMat4("model", transform);
			m_ParticleShader->SetVec4("color", color);

			m_quad.Render();
		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
		particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}