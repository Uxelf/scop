#include <scop.hpp>

float textureTransitionValue = 0;
bool is_texture_active = false;

void textureStartTransition(){
    if (textureTransitionValue == 0 || textureTransitionValue == 1)
            is_texture_active = !is_texture_active;
}

void textureTransition(Shader& shader, float delta_time){

    if (is_texture_active && textureTransitionValue < 1){
        textureTransitionValue += delta_time;
        if (textureTransitionValue > 1)
            textureTransitionValue = 1;            
        shader.use();
        shader.setFloat("textureTransitionValue", textureTransitionValue);
    }
    else if (!is_texture_active && textureTransitionValue > 0){
        textureTransitionValue -= delta_time;
        if (textureTransitionValue < 0)
            textureTransitionValue = 0;
        shader.use();
        shader.setFloat("textureTransitionValue", textureTransitionValue);
    }
}