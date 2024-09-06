#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <iostream>
#include <Vector4.hpp>
#include <Vector3.hpp>

#define DEG_TO_RAD(degrees) ((degrees) * (M_PI / 180.0))

class mat4
{
private:
    float _mat[4][4];
public:
    mat4();
    mat4(float n);
    ~mat4();

    void debug();
    const float* value_ptr() const;

    mat4& operator=(const mat4& other);
    mat4 operator+(const mat4& other) const;
    mat4 operator-(const mat4& other) const;
    mat4 operator*(const mat4& other) const;
    vec4 operator*(const vec4& v) const;

    float* operator[](int index);
    const float* operator[](int index) const;

    mat4& translate(const vec3 v);
    mat4& translate(const float x, const float y, const float z);
    mat4& rotate(const float angle, const vec3 axi);
	mat4& scale(const vec3 v);
	mat4& scale(const float x, const float y, const float z);
};


/* 

typedef float Mat4[4][4];

void debug(Mat4 m);
void debug(vec3 v);
void debug(vec4 v);

void identity(Mat4 m);
void identity(Mat4 m, float val);

void add(Mat4 result, const Mat4 a, const Mat4 b);
void sub(Mat4 result, const Mat4 a, const Mat4 b);
void mult(Mat4 result, const Mat4 a, const Mat4 b);
void mult_vec4(vec4 result, const Mat4 m, const vec4 v);

void clone(Mat4 dest, Mat4 src);

void translate(Mat4 m, float x, float y, float z);
void translate(Mat4 m, vec3 vec);
void rotate(Mat4 m, float angle, vec3 axi);
 */

#endif