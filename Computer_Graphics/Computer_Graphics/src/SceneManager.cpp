#include <pch.h>
#include "SceneManager.h"

namespace vlr 
{
	SceneManager* SceneManager::s_instance = nullptr;

	SceneManager::SceneManager()
	{
		m_scenes.clear();
	}

	void SceneManager::AddScene(Scene* sceneToAdd)
	{
		m_scenes.emplace_back(sceneToAdd);
	}
	void SceneManager::RemoveScene(int idx)
	{
		m_scenes.erase(m_scenes.begin() + idx);
	}

	Scene* SceneManager::FindScene(std::string sceneNameToFind)
	{
		Scene* result = nullptr;
		for (auto scene : m_scenes) 
		{
			if (scene->GetSceneName() == sceneNameToFind) 
			{
				result = scene;
				break;
			}
		}
		
		return result;
	}
	Scene* SceneManager::FindScene(int idx)
	{
		if (idx > m_scenes.size())
			return nullptr;

		return m_scenes[idx];
	}
}