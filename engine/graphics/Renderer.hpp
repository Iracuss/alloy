#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../core/Camera.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    void init();
    void render(GLFWwindow* window);
    bool loadGLFWandGlad();
    void loadShaders(std::vector<Vertex>& cubeVertices, unsigned int indices[], int sizeOf);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // GLFWwindow* window;
    Camera renderCamera;

    unsigned int VAO, VBO, EBO; // Might want to just make a getter for these
private:

    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        glm::mat4 GetMatrix() const {
            glm::mat4 m = glm::mat4(1.0f);

            // translate
            m = glm::translate(m, position);
            
            // rotate around x y or z
            m = glm::rotate(m, rotation.x, glm::vec3(1,0,0));
            m = glm::rotate(m, rotation.y, glm::vec3(0,1,0));
            m = glm::rotate(m, rotation.z, glm::vec3(0,0,1));

            // scale
            m = glm::scale(m, scale);
            return m;
        }
    };

    float fov = 45.0f;

    int width, height;
    
    const char* vertexShaderSrc;
    const char* fragmentShaderSrc;

    unsigned int vertexShader;
    unsigned int fragmentShader;

    unsigned int shaderProgram;
};

#endif