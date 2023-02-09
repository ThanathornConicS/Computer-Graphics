#pragma once

#include "Shader.h"
#include "Time.h"
#include "WindowsInput.h"

#include "GeometryBuffer.h"
#include "HDRBuffer.h"
#include "Material.h"
#include "Light.h"

namespace vlr
{
	class Scene
	{
	public:
		Scene(const char* debugName)
			: m_debugName(debugName)
		{}
		virtual ~Scene() {}

		virtual void OnAttach() { L_INFO("Attaching {0}...", this->m_debugName); }
		virtual void OnDetach() { L_INFO("Detaching {0}...", this->m_debugName); }

		virtual void OnUpdate(Time time) {}

		inline const char* GetSceneName() const
		{
			return m_debugName;
		}

	protected:
		const char* m_debugName = "Default_Scene";

		vlr::WindowsInput m_input;

	private:

	};
}
