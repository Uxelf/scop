#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <Matrix4x4.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{

public:
    unsigned int ID;
    bool is_source;

    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const Shader& other);
    ~Shader();
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const mat4& mat) const;
    void setVec3(const std::string& name, const float x, const float y, const float z);
    void setVec3(const std::string& name, const vec3& vec);
};



#endif