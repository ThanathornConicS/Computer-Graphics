#pragma once
#include <pch.h>
#include "Scene.h"

namespace vlr 
{
	class SceneManager
	{
	public:
		inline static SceneManager& GetInstance() 
		{
			if (!s_instance)
				s_instance = new SceneManager();

			return *s_instance;
		}
		virtual ~SceneManager() {}

		SceneManager(const SceneManager& otherInstance) = delete;
		void operator=(SceneManager& otherInstance) = delete;

		void AddScene(Scene* sceneToAdd);
		void RemoveScene(int idx);

		Scene* FindScene(std::string sceneNameToFind);
		Scene* FindScene(int idx);

	protected:
		SceneManager();

	private:
		static SceneManager* s_instance;

		std::vector<Scene*> m_scenes;
		int sceneIdx = -1;

	};
}


