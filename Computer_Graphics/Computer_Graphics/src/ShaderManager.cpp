#include <pch.h>
#include "ShaderManager.h"

namespace vlr
{
	ShaderManager* ShaderManager::s_instance = nullptr;

	ShaderManager::ShaderManager()
	{
		//ScanAllShaderFiles();
		m_shaders.clear();
	}

	void ShaderManager::ScanAllShaderFiles()
	{
		std::filesystem::path path{ "Shader" };
		std::vector<std::string> shaderPathStorage;
		L_SYSTEM_TRACE("Entries:");
		int count = 1;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string entryPath = (entry.path()).string();
			shaderPathStorage.push_back(entryPath);
			L_SYSTEM_TRACE("Entry #{0}: {1}", count, entryPath);
			count++;
		}
	}
	void ShaderManager::AddShader(const std::string& shaderName, Shader* shaderToAdd)
	{
		m_shaders.insert({shaderName, shaderToAdd});
	}
	void ShaderManager::RemoveShader(std::string& shaderNameToRemove)
	{
		m_shaders.erase(shaderNameToRemove);
	}

	Shader* ShaderManager::FindShader(std::string shaderNameToFind)
	{
		return m_shaders.find(shaderNameToFind)->second;
	}

	void ShaderManager::ListAll()
	{
		int idx = 0;
		for (auto shader : m_shaders) 
		{
			L_SYSTEM_INFO("ShaderIdx {0}: {1}", idx, shader.first);
			idx++;
		}
	}

}