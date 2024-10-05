#include <Object.hpp>

// static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output);

Object::Object(const std::string& obj_path, const Material& material): _material(material)
{
    _scale = vec3(1, 1, 1);
    _model = mat4(1);

    std::vector<float> vertices = loadObjFile(obj_path);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    _elements_count = vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Object::~Object()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Object::render(){
    _material.useShader();

    _material.getShader().setMat4("model", _model);
    _material.setGenericShadersUniforms();
    glBindVertexArray(_VAO);

    glDrawArrays(GL_TRIANGLES, 0, _elements_count);
    
    glBindVertexArray(0);
}

void Object::setPosition(const vec3& new_position){
    _position = new_position;
    _model.translate(_position);
}

void Object::setRotation(const vec3& new_rotation){
    _rotation = new_rotation;

    _model = mat4(1);
    _model.scale(_scale);
    if (_rotation.y)
        _model.rotate(_rotation.y, vec3(0, 1, 0));
    if (_rotation.x)
        _model.rotate(_rotation.x, vec3(1, 0, 0));
    if (_rotation.z)
        _model.rotate(_rotation.z, vec3(0, 0, 1));
    _model.translate(_position);
}

void Object::setScale(const vec3& new_scale){
    _model.scale(1.0f / _scale.x, 1.0f / _scale.y, 1.0f / _scale.z);
    _scale = new_scale;
    _model.scale(_scale);
}

void Object::move(const vec3& movement){
    _position = _position + movement;
    _model.translate(_position);
}

void Object::rotate(const vec3& rotation){
    _model.translate(0, 0, 0);
    if (rotation.y)
        _model.rotate(rotation.y, vec3(0, 1, 0));
    if (rotation.x)
        _model.rotate(rotation.x, vec3(1, 0, 0));
    if (rotation.z)
        _model.rotate(rotation.z, vec3(0, 0, 1));
    _model.translate(_position);
}

void Object::scale(const vec3& scale_amount){
    _scale.x *= scale_amount.x;
    _scale.y *= scale_amount.y;
    _scale.z *= scale_amount.z;
    _model.scale(scale_amount);
}
