#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include "Header/Engine.h"
#include "Header\ResourceManager.h"

using namespace ArduinoCar_Visualization;


// GLFW function declerations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 960;


int main(int argc, char* argv[])
{
    // TODO: Debug code on startup, add compiler define _getcwd()


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create the window
    GLFWwindow* window = glfwCreateWindow(1280, 960, "LearnOpenGL", NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    ResourceManager resourceManager;
    Engine engine(SCREEN_WIDTH, SCREEN_HEIGHT, resourceManager);

    engine.Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Manage user input
        engine.ProcessInput(deltaTime);

        // Update engine state
        engine.Update(deltaTime * 1000.0);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        engine.Render();

        glfwSwapBuffers(window);
    }

    // Clean up resources
    resourceManager.Clear();

    // glfw: Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}