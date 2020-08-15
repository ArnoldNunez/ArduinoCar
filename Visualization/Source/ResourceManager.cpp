#include "..\Header\ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>


using namespace ArduinoCar_Visualization;


ArduinoCar_Visualization::ResourceManager::ResourceManager()
{
}

ArduinoCar_Visualization::ResourceManager::~ResourceManager()
{
}

Shader ArduinoCar_Visualization::ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	this->Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return this->Shaders[name];
}

Shader ArduinoCar_Visualization::ResourceManager::GetShader(std::string name)
{
	return this->Shaders[name];
}

void ArduinoCar_Visualization::ResourceManager::Clear()
{
	// Delete all shaders
	for (auto iter : this->Shaders)
	{
		glDeleteProgram(iter.second.ID);
	}
}


Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}
