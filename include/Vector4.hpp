#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <stdexcept>
#include <math.h>
#include <iostream>

class vec4
{
private:
    float _data[4];
public:
    vec4();
    vec4(float x, float y, float z, float w);
    ~vec4();

    const float* value_ptr() const;

    float& operator[](int index);
    const float& operator[](int index) const;
    
    vec4 operator*(const float n) const;
    vec4 operator+(const vec4& other) const;
    vec4 operator-(const vec4& other) const;

    vec4 normalized();
};

std::ostream& operator<<(std::ostream& os, const vec4& v);

#endif