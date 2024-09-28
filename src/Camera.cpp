#include <Camera.hpp>

Camera::Camera(const float fov, const float aspect, const float near, const float far):
    _fov(fov), _aspect(aspect), _near(near), _far(far),
    _position(vec3(0, 0, 0)), _front(vec3(0, 0, 1)), _up(vec3(0, 1, 0))
{
    calculateProjectionMatrix();
    calculateViewMatrix();
}

Camera::~Camera(){
}

void Camera::calculateProjectionMatrix(){
    float tan_half_fov = tanf(DEG_TO_RAD(_fov) / 2);

    _perspective_projection[0][0] = 1.0f / (_aspect * tan_half_fov);
    _perspective_projection[1][1] = 1.0f / tan_half_fov;
    _perspective_projection[2][2] = -(_far + _near) / (_far - _near);
    _perspective_projection[2][3] = -1;
    _perspective_projection[3][2] = -(2 * _far * _near) / (_far - _near);
}

void Camera::calculateViewMatrix(){
    vec3 camera_right = cross(_front, _up).normalized();
    vec3 camera_up = cross(camera_right, _front);


    mat4 camera_axis(1);
    camera_axis[0][0] = camera_right.x;
    camera_axis[1][0] = camera_right.y;
    camera_axis[2][0] = camera_right.z;

    camera_axis[0][1] = camera_up.x;
    camera_axis[1][1] = camera_up.y;
    camera_axis[2][1] = camera_up.z;

    camera_axis[0][2] = -_front.x;
    camera_axis[1][2] = -_front.y;
    camera_axis[2][2] = -_front.z;

    mat4 camera_positon(1);
    camera_positon.translate(_position * -1);

    _view = camera_axis * camera_positon;
}

void Camera::lookAt(const vec3& target){
    _front = (target -_position).normalized();
    calculateViewMatrix();
}

void Camera::setFront(const vec3& front){
    _front = (front).normalized();

    calculateViewMatrix();
}