#ifndef ENGINE_H
#define ENGINE_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace ArduinoCar_Visualization
{
	class Engine
	{
	public:
		Engine(unsigned int width, unsigned int height);
		~Engine();
		Engine(const Engine&) = delete;

		void Init();
		void ProcessInput(float dt);
		void Update(float dt);
		void Render();
	private:
		unsigned int mWidth;
		unsigned int mHeight;
	};
}

#endif // !ENGINE_H
