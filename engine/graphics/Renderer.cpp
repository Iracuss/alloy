#include "Renderer.hpp"

// This entire file needs to be broken up into more functions to make it more clear



    std::vector<Vertex> cubeVertices = {
        // 8 unique corners with arbitrary colors (you can pick others)
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}}, // 0
        {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}}, // 1
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}}, // 2
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 0}}, // 3
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 1}}, // 4
        {{ 0.5f, -0.5f,  0.5f}, {0, 1, 1}}, // 5
        {{ 0.5f,  0.5f,  0.5f}, {1, 1, 1}}, // 6
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 0}}, // 7
    };

    // These are the 12 triangles
    std::vector<unsigned int> indices = {
        // FRONT
        4, 5, 6,
        4, 6, 7,

        // BACK
        0, 1, 2,
        0, 2, 3,

        // LEFT
        0, 4, 7,
        0, 7, 3,

        // RIGHT
        1, 5, 6,
        1, 6, 2,

        // TOP
        3, 2, 6,
        3, 6, 7,

        // BOTTOM
        0, 1, 5,
        0, 5, 4
    };



Renderer::Renderer() : width(800), height(600)
{}

Renderer::~Renderer() 
{
    
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if(renderer)
    {
        renderer->width = width;
        renderer->height = height;
    }
    glViewport(0, 0, width, height);
}

// This is stuff that I feel I should put into a shaderManager class
void Renderer::loadShaders()
{
    
}

void Renderer::init()
{
    cubeShader = Shader("cube", "cube");
    loadShaders();
    cubeMesh = Mesh(cubeVertices, indices);
}

void Renderer::render(GLFWwindow* window)
{
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubeShader.use();

    glm::mat4 projection = glm::perspective(
        glm::radians(fov),
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    // issue?
    glm::mat4 view = renderCamera.cameraView();

    Transform cube1;
    cube1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    cube1.rotation = glm::vec3(0.0f);
    cube1.scale = glm::vec3(1.0f);

    Transform cube2;
    cube2.position = glm::vec3(3.0f, 0.0f, 0.0f);
    cube2.rotation = glm::vec3(0.0f);
    cube2.scale = glm::vec3(1.0f);

    Transform cubes[] = {cube1, cube2};

    for (int i = 0; i < 2; i++)
    {
        glm::mat4 mvp = projection * view * cubes[i].GetMatrix();

        int mvpLoc = glGetUniformLocation(cubeShader.program, "u_MVP");
        // std::cout << "MVP uniform location: " << mvpLoc << std::endl;

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        cubeMesh.draw();

        // Want to put this in a draw function in mesh
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}
