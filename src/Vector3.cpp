#include <Vector3.hpp>

vec3::vec3(): _data{0, 0, 0}, x(_data[0]), y(_data[1]), z(_data[2])
{
}

vec3::vec3(float x, float y, float z): _data{x, y, z}, x(_data[0]), y(_data[1]), z(_data[2])
{
}

vec3::vec3(const vec3& other): _data{0, 0, 0}, x(_data[0]), y(_data[1]), z(_data[2])
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
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

vec3& vec3::operator=(const vec3& other) {
    if (this != &other) {  // Self-assignment check
        _data[0] = other[0];
        _data[1] = other[1];
        _data[2] = other[2];
    }
    return *this;
}

vec3 vec3::normalized() const{
    float length = sqrt(pow(_data[0], 2) + pow(_data[1], 2) + pow(_data[2], 2));
    return (vec3(_data[0] / length, _data[1] / length, _data[2] / length));
}

vec3 cross(const vec3& a, const vec3& b){
    vec3 result;

    result.x = a.y * b.z - b.y * a.z;
    result.y = a.z * b.x - b.z * a.x;
    result.z = a.x * b.y - b.x * a.y;

    return result;
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}