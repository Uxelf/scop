#include <scop.hpp>


void processInput(GLFWwindow* window, Camera& camera, float delta_time){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    vec3 fp_direction = camera.front();
    fp_direction[1] = 0;
    fp_direction = fp_direction.normalized();
    vec3 movement;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement = movement + fp_direction * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement = movement - fp_direction * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement = movement - camera.right() * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement = movement + camera.right() * CAMERA_SPEED * delta_time;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        movement = movement + vec3(0, 1, 0) * CAMERA_SPEED * delta_time;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        movement = movement - vec3(0, 1, 0) * CAMERA_SPEED * delta_time ;
    
    camera.move(movement);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        if (textureTransitionValue == 0 || textureTransitionValue == 1)
            is_texture_active = !is_texture_active;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        is_light_moving = !is_light_moving;
    }
}