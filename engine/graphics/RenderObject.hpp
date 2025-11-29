#ifndef RENDEROBJ_HPP
#define RENDEROBJ_HPP

#include "../assets/Mesh.hpp"
#include "../assets/Shader.hpp"
#include "../assets/Texture.hpp"
#include "../math/Transform.hpp"

class RenderObject
{
public:
    RenderObject(Mesh& mesh, Shader& shader);
    RenderObject(Mesh& mesh, Shader& shader, Texture& texture);
    RenderObject();
    ~RenderObject();

    Mesh* m_mesh;
    Shader* m_shader;
    Texture* m_texture;
    Transform m_transform;
private:
};

#endif