#ifndef RENDERER_HPP
#define RENDERER_HPP

// #include "../math/Vertex.hpp"
// #include "../math/Cube.hpp"

#include "../core/Camera.hpp"
#include "../assets/Mesh.hpp"
#include "../assets/Shader.hpp"
#include "../math/Transform.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void init();
    void render(GLFWwindow* window);
    bool loadGLFWandGlad();
    void loadShaders();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // GLFWwindow* window;
    Camera renderCamera;
    Mesh cubeMesh;
    Shader cubeShader;

private:
    float fov = 45.0f;

    int width, height;
    
    // const char* vertexShaderSrc;
    // const char* fragmentShaderSrc;

    // unsigned int vertexShader;
    // unsigned int fragmentShader;

    // unsigned int shaderProgram;
};

#endif