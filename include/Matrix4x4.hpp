#ifndef MATRIX4X4_H
#define MATRIX4X4_H

namespace mat4{
    

typedef float Mat4[4][4];
typedef float vec3[3];
typedef float vec4[4];

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

}
#endif