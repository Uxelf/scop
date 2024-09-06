#include <Matrix4x4.hpp>

mat4::mat4(){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            _mat[i][j] = .0f;
    }
}

mat4::mat4(float n){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            _mat[i][j] = (i == j)? n : .0f;
    }
}

mat4::~mat4(){

}

const float* mat4::value_ptr() const {
    return &(_mat[0][0]);
}


void mat4::debug(){
    for (int y = 0; y < 4; y++){
        if (y == 0)
            std::cout << "╔ ";
        else if (y == 3)
            std::cout << "╚ ";
        else
            std::cout << "| ";
        for (int x = 0; x < 4; x++){
            std::cout << _mat[x][y];
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

mat4& mat4::operator=(const mat4& other){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _mat[i][j] = other[i][j];
    return *this;
}

mat4 mat4::operator+(const mat4& other) const{
    mat4 m;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = _mat[i][j] + other[i][j];
    return m;
}

mat4 mat4::operator-(const mat4& other) const{
    mat4 m;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = _mat[i][j] - other[i][j];
    return m;
}

mat4 mat4::operator*(const mat4& other) const{
    mat4 m;
    for (int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            m[x][y] = .0f;
            for (int i = 0; i < 4; i++)
                m[x][y] += _mat[i][x] * other[y][i];
        }
    }
    return m;
}


vec4 mat4::operator*(const vec4& v) const{
    vec4 result;
    for (int y = 0; y < 4; y++){
        result[y] = 0.0f;
        for (int x = 0; x < 4; x++)
            result[y] += _mat[x][y] * v[x];
    }
    return result;
}

float* mat4::operator[](int index) {
    return _mat[index];
}

const float* mat4::operator[](int index) const {
    return _mat[index];
}

mat4& mat4::translate(const vec3 v){
    _mat[3][0] += v[0];
    _mat[3][1] += v[1];
    _mat[3][2] += v[2];
    
    return *this;
}

mat4& mat4::translate(const float x, const float y, const float z){
    _mat[3][0] += x;
    _mat[3][1] += y;
    _mat[3][2] += z;
    
    return *this;
}

mat4& mat4::rotate(const float angle, const vec3 axi){

    float const a = DEG_TO_RAD(angle);
    float c = cos(a);
    float const s = sin(a);

    vec3 axis(axi.normalized());
    vec3 temp(axis * (1.0f - c));

    mat4 Rotate;
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    mat4 Result;

    for (int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            Result[x][y] = .0f;
            for (int i = 0; i < 3; i++)
                Result[x][y] += _mat[i][x] * Rotate[y][i];
        }
    }

    Result[3][0] = _mat[3][0];
    Result[3][1] = _mat[3][1];
    Result[3][2] = _mat[3][2];
    
    (*this) = Result;

    return *this;
}


mat4& mat4::scale(const vec3 v){
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
            _mat[x][y] *= v[y];
    return *this;
}

mat4& mat4::scale(const float x, const float y, const float z){
    return (scale(vec3(x, y, z)));
}



/* void mat4::debug(vec4 v){
    std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << std::endl;
} */

/* void mat4::identity(Mat4 m){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            m[i][j] = (i == j)? 1.0f : .0f;
    }
} */

/* void mat4::identity(Mat4 m, float val){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            m[i][j] = (i == j)? val : .0f;
    }
} */

/* void mat4::add(Mat4 result, const Mat4 a, const Mat4 b){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = a[i][j] + b[i][j];
} */

/* void mat4::sub(Mat4 result, const Mat4 a, const Mat4 b){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = a[i][j] - b[i][j];
} */

/* void mat4::mult(Mat4 result, const Mat4 a, const Mat4 b){
    
    for (int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            result[x][y] = .0f;
            for (int i = 0; i < 4; i++)
                result[x][y] += a[i][x] * b[y][i];
        }
    }
} */

/* void mat4::mult_vec4(vec4 result, const Mat4 m, const vec4 v){
    for (int y = 0; y < 4; y++){
        result[y] = 0.0f;
        for (int x = 0; x < 4; x++)
            result[y] += m[x][y] * v[x];
    }
} */

/* void mat4::clone(Mat4 dest, Mat4 src){
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            dest[i][j] = src[i][j];
} */

/* void mat4::translate(Mat4 m, float x, float y, float z){
    m[3][0] += x;
    m[3][1] += y;
    m[3][2] += z;
} */

/* void mat4::translate(Mat4 m, vec3 v){
    m[3][0] += v[0];
    m[3][1] += v[1];
    m[3][2] += v[2];
} */

/* void mat4::rotate(Mat4 m, float angle, vec4 axi){

} */
