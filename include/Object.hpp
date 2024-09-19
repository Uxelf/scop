#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Material.hpp>
#include <Matrix4x4.hpp>
#include <Vector3.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Object
{
private:
    Material    _material;
    vec3        _position;
    vec3        _rotation;
    vec3        _scale;
    mat4        _model;

    unsigned int    _VAO;
    unsigned int    _VBO;
    /* unsigned int    _EBO;
    bool            _index_rendering; */
    unsigned int    _elements_count;
  
public:
    Object(const std::string& obj_path, const Material& material);
    ~Object();

    void render();

    const mat4&  getModel() const {return _model;}
    const vec3  getPosition() const {return _position;}
    const vec3  getRotation() const {return _rotation;}
    const vec3  getScale() const {return _scale;}

    void setColor(const vec3& new_color) {_material.setColor(new_color);}
    void setPosition(const vec3& new_position);
    void setRotation(const vec3& new_rotation);
    void setScale(const vec3& new_scale);

    void move(const vec3& movement);
    void rotate(const vec3& rotation);
    void scale(const vec3& scale_amount);
};

#endif