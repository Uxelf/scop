#include <scop.hpp>

unsigned int createUBO(unsigned int size){
    unsigned int UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return UBO;
}

void uniformBlockBind(int shader_ID, const std::string& name, GLuint binding_point){
    unsigned int uniform_block_index = glGetUniformBlockIndex(shader_ID, name.c_str());
    glUniformBlockBinding(shader_ID, uniform_block_index, binding_point);
}

void setLightBufferValues(unsigned int UBO_lights, vec3& ambient_light_color, point_light& light){
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_lights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, VEC3_SIZE, ambient_light_color.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * VEC3_SIZE, VEC3_SIZE, light.color.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * VEC3_SIZE, VEC3_SIZE, light.position.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * VEC3_SIZE, VEC3_SIZE, light.diffuse.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE, sizeof(float), &light.constant);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE + sizeof(float), sizeof(float), &light.linear);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE + sizeof(float) * 2, sizeof(float), &light.quadratic);
}