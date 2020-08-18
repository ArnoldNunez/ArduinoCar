#include "..\Header\Engine.h"


using namespace ArduinoCar_Visualization;

Engine::Engine(unsigned int width, unsigned int height, ResourceManager& resourceManager)
	: mWidth(width), mHeight(height), mResourceManager(resourceManager)
{
}

Engine::~Engine()
{

}

void Engine::Init()
{
	// Load shaders
	this->mResourceManager.LoadShader("../Shaders/vertex_shader.vert", "../Shaders/fragment_shader.frag",
		nullptr, "sprite");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->mWidth),
		static_cast<float>(this->mHeight), 0.0f, -1.0f, 1.0f);

	this->mResourceManager.GetShader("sprite").Use().SetInteger("image", 0);
	this->mResourceManager.GetShader("sprite").SetMatrix4("projection", projection);

	// Set render specific controls
	Shader shader = this->mResourceManager.GetShader("sprite");
	this->mRenderer = new SpriteRenderer(shader);

	// Load textures
	this->mResourceManager.LoadTexture("textures/awsomeface.png", true, "face");

}

void Engine::Update(float dt)
{

}

void Engine::ProcessInput(float dt)
{

}

void Engine::Render()
{

}