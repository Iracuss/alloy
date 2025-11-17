#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <vector>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool init();
    void render();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    GLFWwindow* window;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

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

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 400; // window center
    float lastY = 300;
    bool firstMouse = true;

    float fov = 45.0f;

    int width, height;
    
    const char* vertexShaderSrc;
    const char* fragmentShaderSrc;

    unsigned int vertexShader;
    unsigned int fragmentShader;

    unsigned int shaderProgram;

};

#endif