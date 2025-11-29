#include "Mesh.hpp"

Mesh::Mesh() {}

Mesh::Mesh(const std::vector<Vertex>& v, const std::vector<unsigned int>& i) : vertices(v), indicies(i)
{
    indexCount = i.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Vertex), v.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_STATIC_DRAW);

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
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(Vertex), 
        (void*)offsetof(Vertex, uv)
    );

    std::cout << "VAO: " << VAO << " VBO: " << VBO << " EBO: " << EBO << std::endl;


    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
}

Mesh::~Mesh()
{
    // shutdown(); // This is causing an issue with rendering
}

void Mesh::shutdown()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);
    // std::cout << "Drawing with VAO: " << VAO << " indexCount: " << indexCount << std::endl;

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}