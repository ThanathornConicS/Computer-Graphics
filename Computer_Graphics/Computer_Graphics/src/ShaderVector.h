#pragma once

#include <pch.h>
#include "Shader.h"

namespace vlr 
{
	class ShaderVector 
	{
	public:
		inline static ShaderVector& GetInstance()
		{
			if (s_instance == nullptr)
				s_instance = new ShaderVector();

			return *s_instance;
		}

		~ShaderVector();

	protected:
		ShaderVector();

		ShaderVector(const ShaderVector& otherInstance) = delete;
		void operator=(const ShaderVector& otherInstance) = delete;

	private:
		static ShaderVector* s_instance;

		std::unordered_map<std::string, Shader> m_ShaderVector;
	};
}
