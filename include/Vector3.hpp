#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <stdexcept>
#include <math.h>
#include <iostream>

#define VEC3_SIZE sizeof(float) * 4

class vec3
{
private:
    float _data[3];
public:
    float &x, &y, &z;
    vec3();
    vec3(const vec3& other);
    vec3(float x, float y, float z);
    ~vec3();

    const float* value_ptr() const;
    
    float& operator[](int index);
    const float& operator[](int index) const;

    vec3 operator*(const float n) const;
    vec3 operator+(const vec3& other) const;
    vec3 operator-(const vec3& other) const;
    vec3& operator=(const vec3& other);

    vec3 normalized() const;
};

vec3 cross(const vec3& a, const vec3& b);
std::ostream& operator<<(std::ostream& os, const vec3& v);
#endif