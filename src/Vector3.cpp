#include <Vector3.hpp>

vec3::vec3()
{
    _data[0] = 0;
    _data[1] = 0;
    _data[2] = 0;
}

vec3::vec3(float x, float y, float z)
{
    _data[0] = x;
    _data[1] = y;
    _data[2] = z;
}

vec3::~vec3()
{
}

const float* vec3::value_ptr() const{
    return &(_data[0]);
}

float& vec3::operator[](int index) {
    if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
    return _data[index];
}

const float& vec3::operator[](int index) const {
    if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
    return _data[index];
}

vec3 vec3::operator*(const float n) const{
    return (vec3(
    _data[0] * n,
    _data[1] * n,
    _data[2] * n
    ));
}

vec3 vec3::operator+(const vec3& other) const{
    return (vec3(
    _data[0] + other[0],
    _data[1] + other[1],
    _data[2] + other[2]
    ));
}

vec3 vec3::operator-(const vec3& other) const{
    return (vec3(
    _data[0] - other[0],
    _data[1] - other[1],
    _data[2] - other[2]
    ));
}

vec3 vec3::normalized() const{
    float length = sqrt(pow(_data[0], 2) + pow(_data[1], 2) + pow(_data[2], 2));
    return (vec3(_data[0] / length, _data[1] / length, _data[2] / length));
}

vec3 cross(const vec3& a, const vec3& b){
    vec3 result;

    result[0] = a[1] * b[2] - b[1] * a[2];
    result[1] = a[2] * b[0] - b[2] * a[0];
    result[2] = a[0] * b[1] - b[0] * a[1];

    return result;
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    return os;
}