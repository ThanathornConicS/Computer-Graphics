#ifndef SCENE_H
#define SCENE_H

#include "Shader.h"
#include "Time.h"
#include "Input.h"
#include "Window.h"

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

	protected:
		const char* m_debugName = "Default_Scene";
		Input m_input;

	private:

	};
}

#endif // !SCENE_H
