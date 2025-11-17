#include "Engine.hpp"
#include <iostream>

Engine::Engine() : m_isRunning(false)
{}

Engine::~Engine() 
{
    shutdown();
}

// Probably want this to be in a input.cpp file
void Engine::processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        render.cameraPos += cameraSpeed * render.cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        render.cameraPos -= cameraSpeed * render.cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        render.cameraPos -= glm::normalize(glm::cross(render.cameraFront, render.cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        render.cameraPos += glm::normalize(glm::cross(render.cameraFront, render.cameraUp)) * cameraSpeed;
    }

}

void Engine::init()
{
    m_isRunning = render.init();
}

void Engine::update()
{

}

void Engine::shutdown()
{
    glDeleteVertexArrays(1, &render.VAO);
    glDeleteBuffers(1, &render.VBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    m_isRunning = false;
}

void Engine::run()
{
    init();
    if(m_isRunning)
    {
        //gets in here before exiting
        while(!glfwWindowShouldClose(render.window))
        {
            currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(render.window);

            //update?

            render.render();

        }
    }
    shutdown();
}