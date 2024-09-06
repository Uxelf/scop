#ifndef VECTOR3_H
#define VECTOR3_H

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

    vec3& operator*(const float n);

    vec3 normalized() const;
};

std::ostream& operator<<(std::ostream& os, const vec3& v);
#endif