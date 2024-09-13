#include <Object.hpp>

static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output);
static std::vector<std::string> split(const std::string& s, char delimiter);
static std::vector<float> generateUvs(const std::vector<vec3> vertices_vector);
static std::vector<vec3> generateNormalsFromFaces(const std::vector<vec3>& vertices, const std::vector<unsigned int>& indices);

Object::Object(const std::string& obj_path, const Material material): _material(material)
{
    _scale = vec3(1, 1, 1);
    _model = mat4(1);

    std::vector<float> vertices_vector;
    std::vector<unsigned int> indices_vector;

    loadObjFile(obj_path, &vertices_vector, &indices_vector);

    float vertices[vertices_vector.size()];
    unsigned int indices[indices_vector.size()];

    for (unsigned int i = 0; i < vertices_vector.size(); i++)
        vertices[i] = vertices_vector[i];
    for (unsigned int i = 0; i < indices_vector.size(); i++)
        indices[i] = indices_vector[i];

    _index_rendering = (indices_vector.size() != 0);

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


    if (_index_rendering){
        glGenBuffers(1, &_EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        _elements_count = indices_vector.size();
    } else
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

    if (_index_rendering)
        glDrawElements(GL_TRIANGLES, _elements_count, GL_UNSIGNED_INT, 0);
    else
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
    if (_rotation[1])
        _model.rotate(_rotation[1], vec3(0, 1, 0));
    if (_rotation[0])
        _model.rotate(_rotation[0], vec3(1, 0, 0));
    if (_rotation[2])
        _model.rotate(_rotation[2], vec3(0, 0, 1));
    _model.translate(_position);
}

void Object::setScale(const vec3& new_scale){
    _model.scale(1.0f / _scale[0], 1.0f / _scale[1], 1.0f / _scale[2]);
    _scale = new_scale;
    _model.scale(_scale);
}

void Object::move(const vec3& movement){
    _position = _position + movement;
    _model.translate(_position);
}

void Object::rotate(const vec3& rotation){
    _model.translate(0, 0, 0);
    if (rotation[1])
        _model.rotate(rotation[1], vec3(0, 1, 0));
    if (rotation[0])
        _model.rotate(rotation[0], vec3(1, 0, 0));
    if (rotation[2])
        _model.rotate(rotation[2], vec3(0, 0, 1));
    _model.translate(_position);
}

static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output){
    if (path == ""){
        std::cout << "obj path is empty";
        exit(1);
    }
    else{
        std::cout << "OBJ file " << path << " reading in proccess" << std::endl; 
    }

    std::ifstream file(path.c_str());
    if (!file.is_open()){
        std::cerr << "OBJ ERROR reading file " << path << std::endl;
        exit(1);
    }

    std::vector<vec3> vertices;
    std::vector<float> uvs;
    std::vector<vec3> normals;
    std::vector<unsigned int> indices;

    std::string line;
    while(std::getline(file, line)){
        std::vector<std::string> parts = split(line, ' ');

        if (parts.size() == 4 && parts[0] == "v") {
            try {
                float x = std::stof(parts[1]);
                float y = std::stof(parts[2]);
                float z = std::stof(parts[3]);

                vertices.push_back(vec3(x, y, z));
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid float value in line: " << line << std::endl;
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Float value out of range in line: " << line << std::endl;
            }
        }
        else if (parts.size() >= 4 && parts[0] == "f"){
            try
            {
                indices.push_back(std::stoul(parts[1]) - 1);
                indices.push_back(std::stoul(parts[2]) - 1);
                indices.push_back(std::stoul(parts[3]) - 1);
                if (parts.size() == 5){
                    indices.push_back(std::stoul(parts[1]) - 1);
                    indices.push_back(std::stoul(parts[3]) - 1);
                    indices.push_back(std::stoul(parts[4]) - 1);
                }
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid unsigned int value in line: " << line << std::endl;
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Unsigned int value out of range in line: " << line << std::endl;
            }            
        }
    }
    file.close();

    vec3 middle;
    for (unsigned int i = 0; i < vertices.size(); i++)
        middle = middle + vertices[i];
    middle[0] /= vertices.size();
    middle[1] /= vertices.size();
    middle[2] /= vertices.size();
    for (unsigned int i = 0; i < vertices.size(); i++)
        vertices[i] = vertices[i] - middle;

    if (normals.size() == 0){
        if (indices.size() != 0)
            normals = generateNormalsFromFaces(vertices, indices);
        /* else
            normals; */
    }

    uvs = generateUvs(vertices);

    for (unsigned int i = 0; i < vertices.size(); i++){
        vertices_output->push_back(vertices[i][0]);
        vertices_output->push_back(vertices[i][1]);
        vertices_output->push_back(vertices[i][2]);
        vertices_output->push_back(normals[i][0]);
        vertices_output->push_back(normals[i][1]);
        vertices_output->push_back(normals[i][2]);
        vertices_output->push_back(uvs[i]);
        vertices_output->push_back(uvs[i + 1]);
    }
    for (unsigned int i = 0; i < indices.size(); i++)
        indices_output->push_back(indices[i]);
}

static std::vector<std::string> split(const std::string& s, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);

    while (std::getline(token_stream, token, delimiter)){
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

static std::vector<float> generateUvs(const std::vector<vec3> vertices_vector){
    std::vector<float> uvs;

    for (unsigned int i = 0; i < vertices_vector.size(); i++){
        uvs.push_back(0);
        uvs.push_back(0);
    }

    return uvs;
}

static std::vector<vec3> generateNormalsFromFaces(const std::vector<vec3>& vertices, const std::vector<unsigned int>& indices){
    std::vector<vec3> normals(vertices.size());

    for (unsigned int i = 0; i < indices.size(); i+=3){
        vec3 A = vertices[indices[i]];
        vec3 B = vertices[indices[i + 1]];
        vec3 C = vertices[indices[i + 2]];

        vec3 dir = cross((B - A).normalized(), (C - A).normalized());
        normals[indices[i]] = normals[indices[i]] + dir;
        normals[indices[i + 1]] = normals[indices[i + 1]] + dir;
        normals[indices[i + 2]] = normals[indices[i + 2]] + dir;
    }
    for (unsigned int i = 0; i < normals.size(); i++){
        normals[i] = normals[i].normalized();
    }
    return normals;
}