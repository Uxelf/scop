#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Matrix4x4.hpp>
#include <Vector3.hpp>


class Camera
{
private:
    float   _fov;
    float   _aspect;
    float   _near;
    float   _far;

    vec3    _position;
    vec3    _front;
    vec3    _up;

    float   _pitch;
    float   _yaw;
    
    mat4    _perspective_projection;
    mat4    _view;

    void    calculateProjectionMatrix();
    void    calculateViewMatrix();
public:
    Camera(const float fov, const float aspect, const float near, const float far);
    ~Camera();

    const mat4& getPerspectiveProjection() const {return _perspective_projection;}
    const mat4& getViewMatrix() const {return _view;}
    float getFov() const {return _fov;}
    float getAspect() const {return _aspect;}

    void setFov(const float fov) {_fov = fov; calculateProjectionMatrix();}
    void setAspect(const float aspect) {_aspect = aspect; calculateProjectionMatrix();}
    void setNearPlane(const float near) {_near = near; calculateProjectionMatrix();}
    void setFarPlane(const float far) {_far = far; calculateProjectionMatrix();}
    
    void move(const vec3& movement_vector) {_position = _position + movement_vector; calculateViewMatrix();};
    void moveTo(const vec3& position) {_position = position; calculateViewMatrix();}
    void lookAt(const vec3& target);
    void setFront(const vec3& front);

    const vec3 up() {return _up;}
    const vec3 front() {return _front;}
    const vec3 right() {return cross(_front, _up).normalized();}
    const vec3 position() {return _position;}
    float pitch() {return _pitch;}
    float yaw() {return _yaw;}
};

#endif