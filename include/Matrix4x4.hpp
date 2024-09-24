#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <iostream>
#include <Vector4.hpp>
#include <Vector3.hpp>

#define MAT4_SIZE sizeof(float) * 16
constexpr float DEG_TO_RAD(float degrees){return ((degrees) * (M_PI / 180.0));}
constexpr float RAD_TO_DEG(float rads){return ((rads) * (180.0 / M_PI));}

class mat4
{
private:
    float _mat[4][4];
public:
    mat4();
    mat4(const mat4& other);
    mat4(float n);
    ~mat4();

    void debug();
    const float* value_ptr() const;

    mat4& operator=(const mat4& other);
    mat4 operator+(const mat4& other) const;
    mat4 operator-(const mat4& other) const;
    mat4 operator*(const mat4& other) const;
    vec4 operator*(const vec4& v) const;
    mat4 operator*(const float& n) const;

    float* operator[](int index);
    const float* operator[](int index) const;

    mat4& translate(const vec3 v);
    mat4& translate(const float x, const float y, const float z);
    mat4& rotate(const float angle, const vec3 axi);
	mat4& scale(const vec3 v);
	mat4& scale(const float x, const float y, const float z);
};


#endif