#include <Vector3.hpp>

vec3::vec3()
{
}

vec3::vec3(float x, float y, float z)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

vec3::~vec3()
{
}

float& vec3::operator[](int index) {
    if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
    return data[index];
}

const float& vec3::operator[](int index) const {
    if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
    return data[index];
}

vec3& vec3::operator*(const float n){
    data[0] *= n;
    data[1] *= n;
    data[2] *= n;
    return *this;
}

vec3 vec3::normalized() const{
    float length = sqrt(pow(data[0], 2) + pow(data[1], 2) + pow(data[2], 2));
    return (vec3(data[0] / length, data[1] / length, data[2] / length));
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    return os;
}