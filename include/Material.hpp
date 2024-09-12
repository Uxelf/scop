#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Shader.hpp>
#include <Vector3.hpp>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Texture{
    unsigned int ID;
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
};

class Material
{
private:
    Shader  _shader;
    vec3    _color;
    Texture _texture;
    bool    _use_texture;

public:
    Material(const Shader& shader, const vec3& color, const std::string& texturePath);
    Material(const Material& other);
    ~Material();

    void setColor(const vec3& color) {_color = color;}

    const Shader& getShader() const {return _shader;}
    const vec3    getColor() const {return _color;}
    const Texture getTexture() const {return _texture;}

    void useShader() {_shader.use();}
};

#endif