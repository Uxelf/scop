#include <Object.hpp>

static void loadObjFile(const std::string& path, float* vertices_output, float* indices_output);
static std::vector<std::string> split(const std::string& s, char delimiter);
static std::vector<float> generateUvs(const std::vector<float> vertices_vector);

Object::Object(const std::string& obj_path, const Material material): _material(material)
{
    _scale = vec3(1, 1, 1);
    _model = mat4(1);

    std::vector<float> vertices_vector;
    std::vector<float> indices_vector;

    loadObjFile(obj_path, &vertices_vector, &indices_vector);

    float vertices[vertices_vector.size()];
    float indices[indices_vector.size()];

    for (int i = 0; i < vertices_vector.size(); i++)
        vertices[i] = vertices_vector[i];
    for (int i = 0; i < indices_vector.size(); i++)
        indices[i] = indices_vector[i];

    _index_rendering = (indices_vector.size() != 0);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    if (_index_rendering){
        glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &_EBO);

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

    glBindVertexArray(_VAO);

    if (_index_rendering)
        glDrawElements(GL_TRIANGLES, _elements_count, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, _elements_count);
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

static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<float>* indices_output){
    if (path == ""){
        std::cout << "obj path is empty";
        exit(1);
    }
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try{
        file.open(path);
    }
    catch(std::ifstream::failure e){
        std::cerr << "OBJ ERROR reading file " << path << std::endl;
        exit(1);
    }

    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<float> indices;

    std::string line;
    int i = 0;
    while(std::getline(file, line)){
        i++;
        if (line[0] == 'v'){
            std::istringstream iss(line.substr(1));
            float x, y, z;

            // Try to parse exactly three float numbers
            if ((iss >> x >> y >> z)) {
                // Check if there is no extra data after the three floats
                std::string extra;
                if (!(iss >> extra)) {  // This ensures no extra characters or numbers
                    // If parsing is successful, store them in the vertices vector
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                } else {
                    std::cerr << "Error: Line " << i << " has more than 3 float numbers or extra data: " << line << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "Error: Line " << i << " does not contain 3 valid float numbers: " << line << std::endl;
                exit(1);
            }
        }
        else if (line[0] == 'f'){

        }
    }

    uvs = generateUvs(vertices);

    for (int i = 0; i < vertices.size(); i += 3){
        vertices_output->push_back(vertices[i]);
        vertices_output->push_back(vertices[i + 1]);
        vertices_output->push_back(vertices[i + 2]);
        vertices_output->push_back(normals[i]);
        vertices_output->push_back(normals[i + 1]);
        vertices_output->push_back(normals[i + 2]);
        vertices_output->push_back(uvs[i]);
        vertices_output->push_back(uvs[i + 1]);
        vertices_output->push_back(uvs[i + 2]);
    }

    for (int i = 0; i < indices.size(); i++)
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


static std::vector<float> generateUvs(const std::vector<float> vertices_vector){
    std::vector<float> uvs;

    for (int i = 0; i < vertices_vector.size(); i++){
        uvs.push_back(0);
    }

    return uvs;
}