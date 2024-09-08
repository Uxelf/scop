#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <stdexcept>
#include <math.h>
#include <iostream>

class vec3
{
private:
    float data[3];
public:
    vec3();
    vec3(float x, float y, float z);
    ~vec3();

    float& operator[](int index);
    const float& operator[](int index) const;

    vec3 operator*(const float n) const;
    vec3 operator+(const vec3& other) const;
    vec3 operator-(const vec3& other) const;

    vec3 normalized() const;
};

vec3 cross(const vec3& a, const vec3& b);
std::ostream& operator<<(std::ostream& os, const vec3& v);
#endif