#include <scop.hpp>


void processInput(GLFWwindow* window, Camera& camera, float delta_time){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    vec3 fpsDirection;
    fpsDirection[0] = cos(DEG_TO_RAD(camera.yaw()));
    fpsDirection[2] = sin(DEG_TO_RAD(camera.yaw()));
    fpsDirection = fpsDirection.normalized();
    vec3 movement;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement = movement + fpsDirection * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement = movement + fpsDirection * CAMERA_SPEED * delta_time * -1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement = movement + camera.right() * CAMERA_SPEED * delta_time * -1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement = movement + camera.right() * CAMERA_SPEED * delta_time;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        movement = movement + vec3(0, 1, 0) * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        movement = movement + vec3(0, 1, 0) * CAMERA_SPEED * delta_time * -1;
    
    camera.move(movement);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        if (textureTransitionValue == 0 || textureTransitionValue == 1)
            is_texture_active = !is_texture_active;
    }

}