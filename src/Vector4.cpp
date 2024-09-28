#include <Vector4.hpp>

vec4::vec4(): _data{0, 0, 0, 0}, x(_data[0]), y(_data[1]), z(_data[2]), w(_data[3])
{
}

vec4::vec4(float x, float y, float z, float w): _data{x, y, z, w},
x(_data[0]), y(_data[1]), z(_data[2]), w(_data[3])
{
}

vec4::vec4(const vec4& other): _data{0, 0, 0, 0}, x(_data[0]), y(_data[1]), z(_data[2]), w(_data[3])
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
}

vec4::~vec4()
{
}

const float* vec4::value_ptr() const{
    return &(_data[0]);
}

float& vec4::operator[](int index) {
    if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
    return _data[index];
}

const float& vec4::operator[](int index) const {
    if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
    return _data[index];
}

vec4 vec4::operator*(const float n) const{
    return (vec4(
    _data[0] * n,
    _data[1] * n,
    _data[2] * n,
    _data[3] * n
    ));
}

vec4 vec4::operator+(const vec4& other) const{
    return (vec4(
    _data[0] + other[0],
    _data[1] + other[1],
    _data[2] + other[2],
    _data[3] + other[3]
    ));
}

vec4 vec4::operator-(const vec4& other) const{
    return (vec4(
    _data[0] - other[0],
    _data[1] - other[1],
    _data[2] - other[2],
    _data[3] - other[3]
    ));
}


vec4 vec4::normalized(){
    float length = sqrt(pow(_data[0], 2) + pow(_data[1], 2) + pow(_data[2], 2) + pow(_data[3], 2));
    return (vec4(_data[0] / length, _data[1] / length, _data[2] / length, _data[3] / length));
}

std::ostream& operator<<(std::ostream& os, const vec4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}