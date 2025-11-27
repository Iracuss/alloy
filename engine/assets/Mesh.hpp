#ifndef MESH_HPP
#define MESH_HPP

#include "../math/Vertex.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies);
    Mesh();
    ~Mesh();

    void shutdown();
    void draw();

    unsigned int indexCount;
    unsigned int VAO, VBO, EBO;
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indicies;
};

#endif