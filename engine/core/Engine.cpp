#include "Engine.hpp"
#include <iostream>

Engine::Engine() : m_isRunning(false)
{}

Engine::~Engine() 
{
    shutdown();
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

    glfwDestroyWindow(render.window);
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
            //maybe put this in a different class all can use so I don't pass it
            currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            render.renderCamera.processInput(render.window, deltaTime);

            //update?

            render.render();

        }
    }
    shutdown();
}