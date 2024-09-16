#include <Matrix4x4.hpp>

mat4::mat4(){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            _mat[i][j] = .0f;
    }
}

mat4::mat4(const mat4& other){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            _mat[i][j] = other[i][j];
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
                m[x][y] += _mat[i][y] * other[x][i];
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

mat4 mat4::operator*(const float& n) const{
    mat4 m;
    for (int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            m[x][y] = _mat[x][y] * n;
        }
    }
    return m;
}

float* mat4::operator[](int index) {
    return _mat[index];
}

const float* mat4::operator[](int index) const {
    return _mat[index];
}

mat4& mat4::translate(const vec3 v){
    _mat[3][0] = v[0];
    _mat[3][1] = v[1];
    _mat[3][2] = v[2];
    
    return *this;
}

mat4& mat4::translate(const float x, const float y, const float z){
    _mat[3][0] = x;
    _mat[3][1] = y;
    _mat[3][2] = z;
    
    return *this;
}

mat4& mat4::rotate(const float angle, const vec3 axi){

    float const a = DEG_TO_RAD(angle);
    float c = cosf(a);
    float const s = sinf(a);

    mat4 R;
    mat4 I(1);
    mat4 K;
    vec3 n_axi = axi.normalized();

    I[3][3] = 0;

    K[1][0] = -n_axi[2];
    K[2][0] = n_axi[1];
    K[0][1] = n_axi[2];
    K[2][1] = -n_axi[0];
    K[0][2] = -n_axi[1];
    K[1][2] = n_axi[0];

    R = I + (K * s) + (K * K) * (1 - c);
    R[3][3] = 1;

    mat4 result;
    result = R * (*this);

    *this = result;

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
