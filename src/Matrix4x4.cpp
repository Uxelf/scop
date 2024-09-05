#include <Matrix4x4.hpp>

#include <iostream>
void mat4::debug(Mat4 m){
    for (int y = 0; y < 4; y++){
        if (y == 0)
            std::cout << "╔ ";
        else if (y == 3)
            std::cout << "╚ ";
        else
            std::cout << "| ";
        for (int x = 0; x < 4; x++){
            std::cout << m[x][y];
            if (x != 3)
                std::cout << ",\t";
        }
        if (y == 0)
            std::cout << "\t╦";
        else if (y == 3)
            std::cout << "\t╩";
        else
            std::cout << "\t|";
        std::cout << std::endl;
    }
}

void mat4::debug(vec4 v){
    std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << std::endl;
}

void mat4::identity(Mat4 m){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            m[i][j] = (i == j)? 1.0f : .0f;
    }
}

void mat4::identity(Mat4 m, float val){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            m[i][j] = (i == j)? val : .0f;
    }
}

void mat4::add(Mat4 result, const Mat4 a, const Mat4 b){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = a[i][j] + b[i][j];
}

void mat4::sub(Mat4 result, const Mat4 a, const Mat4 b){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = a[i][j] - b[i][j];
}

void mat4::mult(Mat4 result, const Mat4 a, const Mat4 b){
    
    for (int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            result[x][y] = .0f;
            for (int i = 0; i < 4; i++)
                result[x][y] += a[i][x] * b[y][i];
        }
    }
}

void mat4::mult_vec4(vec4 result, const Mat4 m, const vec4 v){
    for (int y = 0; y < 4; y++){
        result[y] = 0.0f;
        for (int x = 0; x < 4; x++)
            result[y] += m[x][y] * v[x];
    }
}

void mat4::clone(Mat4 dest, Mat4 src){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            dest[i][j] = src[i][j];
}

void mat4::translate(Mat4 m, float x, float y, float z){
    m[3][0] += x;
    m[3][1] += y;
    m[3][2] += z;
}

void mat4::translate(Mat4 m, vec3 v){
    m[3][0] += v[0];
    m[3][1] += v[1];
    m[3][2] += v[2];
}

void mat4::rotate(Mat4 m, float angle, vec4 axi){

}
