#ifndef SCOP_HPP
#define SCOP_HPP

#include <Material.hpp>
#include <Matrix4x4.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Camera.hpp>
#include <Object.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

#include <Shader.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const float CAMERA_SPEED = 4.5f;
const float CAMERA_SENSIBILITY = 0.004f;

const float OBJECTS_SEPARATION = 8.0f;

extern float textureTransitionValue;
extern bool is_texture_active;
extern bool is_light_moving;

struct point_light{
    vec3 position;
    vec3 color;
    vec3 diffuse;
    float constant;
    float linear;
    float quadratic;
};

GLFWwindow*     createWindow(int width, int height, const std::string& name);
unsigned int    createUBO(unsigned int size);
void            uniformBlockBind(int shader_ID, const std::string& name, GLuint binding_point);
void            setLightBufferValues(unsigned int UBO_lights, vec3& ambient_light_color, point_light& light);
void            processInput(GLFWwindow* window, Camera& camera, float delta_time);

#endif