#include <Object.hpp>

// static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output);

Object::Object(const std::string& obj_path, const Material& material): _material(material)
{
    _scale = vec3(1, 1, 1);
    _model = mat4(1);

    std::vector<float> vertices_vector = loadObjFile(obj_path);
    // std::vector<unsigned int> indices_vector;

    // loadObjFile(obj_path, &vertices_vector, &indices_vector);

    float vertices[vertices_vector.size()];
    // unsigned int indices[indices_vector.size()];

    for (unsigned int i = 0; i < vertices_vector.size(); i++)
        vertices[i] = vertices_vector[i];
    // for (unsigned int i = 0; i < indices_vector.size(); i++)
    //     indices[i] = indices_vector[i];


    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    /* if (_index_rendering){
        glGenBuffers(1, &_EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        _elements_count = indices_vector.size();
    } else */
        _elements_count = vertices_vector.size();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Object::~Object()
{
}

void Object::render(){
    _material.useShader();

    _material.getShader().setMat4("model", _model);
    _material.setGenericShadersUniforms();
    glBindVertexArray(_VAO);

    /* if (_index_rendering)
        glDrawElements(GL_TRIANGLES, _elements_count, GL_UNSIGNED_INT, 0);
    else */
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
