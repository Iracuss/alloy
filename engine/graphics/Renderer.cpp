#include "Renderer.hpp"

// This entire file needs to be broken up into more functions to make it more clear

Renderer::Renderer() : window(nullptr), width(800), height(600)
{}

Renderer::~Renderer() 
{
    
}

void Renderer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if(!renderer) return;

    if(renderer->firstMouse)
    {
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->firstMouse = false;
    }

    float sensitivity = 0.1f;
    float xoffset = (xpos - renderer->lastX) * sensitivity;
    float yoffset = (renderer->lastY - ypos) * sensitivity; // got to reverse it for y

    renderer->lastX = xpos;
    renderer->lastY = ypos;

    renderer->yaw += xoffset;
    renderer->pitch += yoffset;

    // Limit looking up or down
    if(renderer->pitch > 89.0f)
    {
        renderer->pitch = 89.0f;
    }
    if(renderer->pitch < -89.0f)
    {
        renderer->pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(renderer->yaw)) * cos(glm::radians(renderer->pitch));
    direction.y = sin(glm::radians(renderer->pitch));
    direction.z = sin(glm::radians(renderer->yaw)) * cos(glm::radians(renderer->pitch));
    renderer->cameraFront = glm::normalize(direction);
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

bool Renderer::init()
{
    // init glfw
    if(!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create the window
    window = glfwCreateWindow(800, 600, "Alloy renderer", nullptr, nullptr);
    if(!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // keep this here maybe
    glfwSetWindowUserPointer(window, this);

    // load GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to init GLAD\n";
        return false;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide and capture cursor

    glEnable(GL_DEPTH_TEST);


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
    unsigned int indices[] = {
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

    // vbo stores vertexs in the gpu
    // vao stores the vertex data should be interpreted
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(Vertex), cubeVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret vertex data
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,              // layout location = 0
        3,              // 3 floats per vertex
        GL_FLOAT,       // type
        GL_FALSE,       // no normalization
        sizeof(Vertex),  // stride
        (void*)offsetof(Vertex, position)        // offset
    );

    // color attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexShaderSrc = R"(
    #version 330 core

    uniform mat4 u_MVP;
    
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vColor;

    void main() {
        vColor = aColor;
        gl_Position = u_MVP * vec4(aPos, 1.0);
    }
    )";

    fragmentShaderSrc = R"(
    #version 330 core

    in vec3 vColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(vColor, 1.0);
    }
    )";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    //Error checking
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "VERTEX SHADER COMPILE ERROR:\n" << infoLog << std::endl;
    }


    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    infoLog[0] = '\0';
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR:\n" << infoLog << std::endl;
    }


    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Tell OpenGL to resize viewport when window does
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

 
    return true;
}

void Renderer::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(shaderProgram);

    glm::mat4 view = glm::lookAt(
        cameraPos,
        cameraPos + cameraFront,
        cameraUp
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(fov),
        (float)width / (float)height,
        0.1f,
        100.0f
    );


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

        int mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}