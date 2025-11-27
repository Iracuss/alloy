#ifndef SHADER_HPP
#define SHADER_HPP

#include "../utils/LoadFiles.hpp"
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader();
    Shader(const std::string& vertSrc, const std::string& fragSrc);
    ~Shader();

    void use();
    // void setMat4(const std::string& name, const glm::mat4& mat);


    unsigned int program; // Need this to do some linking later
private:
    std::string vertShader;
    std::string fragShader;

    unsigned int vertexShader;
    unsigned int fragmentShader;
};


#endif