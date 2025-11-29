#ifndef SHADER_HPP
#define SHADER_HPP

#include "../utils/LoadFiles.hpp"
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
    Shader();
    Shader(const std::string& vertSrc, const std::string& fragSrc);
    ~Shader();

    void use();
    void setMat4(const std::string& name, const glm::mat4& mat); // work on this
    void setInt(const std::string& name, int val);

    unsigned int program;
private:
    std::string vertShader;
    std::string fragShader;
};


#endif