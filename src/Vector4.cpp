#include <Vector4.hpp>

vec4::vec4()
{
}

vec4::vec4(float x, float y, float z, float w)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}

vec4::~vec4()
{
}

float& vec4::operator[](int index) {
    if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
    return data[index];
}

const float& vec4::operator[](int index) const {
    if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
    return data[index];
}

vec4& vec4::operator*(const float n){
    data[0] *= n;
    data[1] *= n;
    data[2] *= n;
    data[3] *= n;
    return *this;
}

vec4 vec4::normalized(){
    float length = sqrt(pow(data[0], 2) + pow(data[1], 2) + pow(data[2], 2) + pow(data[3], 2));
    return (vec4(data[0] / length, data[1] / length, data[2] / length, data[3] / length));
}

std::ostream& operator<<(std::ostream& os, const vec4& v) {
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
    return os;
}