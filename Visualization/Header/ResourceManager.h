#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad\glad.h>

#include "Shader.h"

namespace ArduinoCar_Visualization
{
	class ResourceManager
	{
	public:
		std::map<std::string, Shader> Shaders;

		ResourceManager();
		~ResourceManager();
		ResourceManager(const ResourceManager&) = delete;

		/**
		 * Loads and generates a shader, fragment, and geometry (if not null) from file.
		 */
		Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

		/**
		 * Gets a shader by name
		 */
		Shader GetShader(std::string name);

		/**
		 * Properly deallocates all loaded resources
		 */
		void Clear();
	private:

		/**
		 * loads and generates a shader from file
		 */
		
		Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	};
}

#endif // !RESOURCE_MANAGER_H
