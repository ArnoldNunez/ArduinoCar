#include "..\Header\Engine.h"


using namespace ArduinoCar_Visualization;

Engine::Engine(unsigned int width, unsigned int height, ResourceManager& resourceManager)
	: mWidth(width), mHeight(height), mResourceManager(resourceManager), mRenderer(nullptr)
{
}

Engine::~Engine()
{
	delete this->mRenderer;
}

void Engine::Init()
{
	// Load shaders
	this->mResourceManager.LoadShader("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag",
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
	this->mResourceManager.LoadTexture("Textures/awesomeface.png", true, "face");
	this->mResourceManager.LoadTexture("Textures/Health.png", true, "gem");
	this->mResourceManager.LoadTexture("Textures/Health.png", true, "gem_empty");
	//this->mResourceManager.LoadTexture("Textures/red_dot_better.png", false, "red_dot");
	//this->mResourceManager.LoadTexture("Textures/red_dot_dark.png", false, "red_dot_dark");
	this->mResourceManager.LoadTexture("Textures/Health.png", true, "turtle");

	// Create Tests
	Simulation simulation1(this->mResourceManager);
	simulation1.FromFile("Simulation1.sim");

	Simulation simulation2(this->mResourceManager);
	simulation2.FromFile("Simulation2.sim");

	Simulation simulation3(this->mResourceManager);
	simulation2.FromFile("Simulation3.sim");

	this->Tests.push_back(simulation1);
	this->Tests.push_back(simulation2);
	this->Tests.push_back(simulation3);
	this->Test = 0;
}

void Engine::Update(float dt)
{
	this->Tests[this->Test].Update(dt);
}

void Engine::ProcessInput(float dt)
{

}

void Engine::Render()
{
	/*Texture2D faceTex = this->mResourceManager.GetTexture("face");
	this->mRenderer->DrawSprite(faceTex, glm::vec2(200.0f, 200.0f),
		glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	Texture2D turtleTex = this->mResourceManager.GetTexture("turtle");
	this->mRenderer->DrawSprite(turtleTex, glm::vec2(0.0f, 0.0f), glm::vec2(150.0f, 150.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));*/

	this->Tests[this->Test].Draw(*mRenderer);
}