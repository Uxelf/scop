#include <scop.hpp>

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