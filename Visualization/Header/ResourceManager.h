#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Shader.h"
#include "Texture2D.h"

#include <glad\glad.h>

#include <map>
#include <string>

namespace ArduinoCar_Visualization
{
	class ResourceManager
	{
	public:
		std::map<std::string, Shader> Shaders;
		std::map<std::string, Texture2D> Textures;

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
		 * Loads (and generates) a texture from file
		 */
		Texture2D LoadTexture(const char* file, bool alpha, std::string name);

		/**
		 * Retreives a stored texture
		 */
		Texture2D GetTexture(std::string name);

		/**
		 * Properly deallocates all loaded resources
		 */
		void Clear();
	private:

		/**
		 * loads and generates a shader from file
		 */
		
		Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

		/**
		 * Loads a single texture from file
		 */
		Texture2D loadTextureFromFile(const char* file, bool alpha);
	};
}

#endif // !RESOURCE_MANAGER_H
