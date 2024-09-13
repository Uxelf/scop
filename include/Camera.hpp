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
    vec3    _target;
    vec3    _up;
    
    mat4    _perspective_projection;
    mat4    _view;

    void    calculateProjectionMatrix();
    void    calculateViewMatrix();
public:
    Camera(const float fov, const float aspect, const float near, const float far);
    ~Camera();

    const mat4& getPerspectiveProjection() const { return _perspective_projection;}
    const mat4& getViewMatrix() const { return _view;}

    void setFov(const float fov) {_fov = fov; calculateProjectionMatrix();}
    void setAspect(const float aspect) {_aspect = aspect; calculateProjectionMatrix();}
    void setNearPlane(const float near) {_near = near; calculateProjectionMatrix();}
    void setFarPlane(const float far) { _far = far; calculateProjectionMatrix();}

    float getFov() const {return _fov;}
    float getAspect() const {return _aspect;}
     
    void lookAt(const vec3& position, const vec3& target, const vec3& up);
};

#endif