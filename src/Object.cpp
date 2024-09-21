#include <Object.hpp>

// static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output);
static std::vector<float> loadObjFile(const std::string& path);
static std::vector<std::string> split(const std::string& s, char delimiter);
static std::vector<std::string> splitFaceInfo(const std::string& s);
/* static std::vector<float> generateUvs(const std::vector<vec3> vertices_vector);
static std::vector<vec3> generateNormalsFromFaces(const std::vector<vec3>& vertices, const std::vector<unsigned int>& indices); */
static std::vector<float> generateVerticesFromData(const std::vector<vec3>& vertices_list, const std::vector<vec3>& normals_list, const std::vector<vec3>& uvs_list, const std::vector<unsigned int>& faces_indices, const std::vector<unsigned int>& normals_indices, const std::vector<unsigned int>& texture_indices);
static std::vector<float> centerVertices(const std::vector<float>& vertices);
static vec3 generateNormal(vec3 v1, vec3 v2, vec3 v3);
static std::vector<vec3> generateUvs(const std::vector<vec3>& vertices);

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

void Object::scale(const vec3& scale_amount){
    _scale[0] *= scale_amount[0];
    _scale[1] *= scale_amount[1];
    _scale[2] *= scale_amount[2];
    _model.scale(scale_amount);
}



// static void loadObjFile(const std::string& path, std::vector<float>* vertices_output, std::vector<unsigned int>* indices_output){
static std::vector<float> loadObjFile(const std::string& path){
    if (path == ""){
        std::cout << "obj path is empty";
        throw(std::runtime_error("Obj path is empty"));
    }

    std::ifstream file(path.c_str());
    if (!file.is_open()){
        std::cerr << "\e[96mOBJ ERROR reading file " << path << std::endl;
        exit(1);
    }

    std::vector<vec3> vertices_list;
    std::vector<vec3> normals_list;
    std::vector<vec3> uvs_list;
    std::vector<unsigned int> faces_indices;
    std::vector<unsigned int> normals_indices;
    std::vector<unsigned int> texture_indices;

    std::string line;
    while(std::getline(file, line)){
        std::vector<std::string> parts = split(line, ' ');
        if (parts.size() == 0)
            continue;
        if (parts.size() == 4 && parts[0] == "v") {
            try {
                float x = std::stof(parts[1]);
                float y = std::stof(parts[2]);
                float z = std::stof(parts[3]);

                vertices_list.push_back(vec3(x, y, z));
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid float value in line: " << line << std::endl;
                std::cerr << e.what();
                return std::vector<float>(0);
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Float value out of range in line: " << line << std::endl;
                std::cerr << e.what();
                return std::vector<float>(0);
            }
        }
        else if (parts[0] == "vn"){
            try
            {
                float x = std::stof(parts[1]);
                float y = std::stof(parts[2]);
                float z = std::stof(parts[3]);

                normals_list.push_back(vec3(x, y, z));
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid float value in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Float value out of range in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }   
        }
        else if (parts[0] == "vt"){
            try
            {
                float x = std::stof(parts[1]);
                float y = std::stof(parts[2]);

                uvs_list.push_back(vec3(x, y, 0));
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid float value in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Float value out of range in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }   
        }
        else if (parts.size() >= 4 && parts[0] == "f"){
            try
            {
                std::vector<std::string> first_face_parts = splitFaceInfo(parts[1]);

                for (unsigned int i = 2; i + 1 < parts.size(); i++){
                    std::vector<std::string> face_parts_1 = splitFaceInfo(parts[i]);
                    std::vector<std::string> face_parts_2 = splitFaceInfo(parts[i + 1]);

                    faces_indices.push_back(std::stoul(first_face_parts[0]) - 1);
                    faces_indices.push_back(std::stoul(face_parts_1[0]) - 1);
                    faces_indices.push_back(std::stoul(face_parts_2[0]) - 1);

                    if (!first_face_parts[1].empty() && !face_parts_1[1].empty() && !face_parts_2[1].empty()){
                        texture_indices.push_back(std::stoul(first_face_parts[1]) - 1);
                        texture_indices.push_back(std::stoul(face_parts_1[1]) - 1);
                        texture_indices.push_back(std::stoul(face_parts_2[1]) - 1);
                    }

                    if (!first_face_parts[2].empty() && !face_parts_1[2].empty() && !face_parts_2[2].empty()){
                        normals_indices.push_back(std::stoul(first_face_parts[2]) - 1);
                        normals_indices.push_back(std::stoul(face_parts_1[2]) - 1);
                        normals_indices.push_back(std::stoul(face_parts_2[2]) - 1);
                    }
                }
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid unsigned int value in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Error: Unsigned int value out of range in line: " << line << std::endl;
                std::cerr << e.what() << std::endl;
                return std::vector<float>(0);
            }            
        }
        
    }

    std::cout << "Closing file" << std::endl;
    file.close();
    
    std::cout << "Generating vertices from data" << std::endl;

    std::vector<float> vertices = generateVerticesFromData(vertices_list, normals_list, uvs_list, faces_indices, normals_indices, texture_indices);
    std::vector<float> centered_vertices = centerVertices(vertices);
    return centered_vertices;
}

static std::vector<std::string> split(const std::string& s, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);

    while (std::getline(token_stream, token, delimiter)){
            tokens.push_back(token);
    }
    return tokens;
}

static std::vector<std::string> splitFaceInfo(const std::string& s){
    std::vector<std::string> tokens(3);
    tokens[0] = "";
    tokens[1] = "";
    tokens[2] = "";

    unsigned int n = 0;
    for (unsigned int i = 0; i < s.size(); i++){
        if (s[i] == '/'){
            n++;
            if (n > 2) //! Puede haber más de 3 elementos por cara??
                break;
            continue;
        }
        tokens[n] += s[i];
    }
    return tokens;
}

static void throwCustomIndexError(const std::string& str, unsigned int index){
    
    std::string indexStr = std::to_string(index);

    size_t pos = str.find("%d");

    std::string error_str = str;
    if (pos != std::string::npos) {
        error_str.replace(pos, 2, indexStr);
    }

    throw std::runtime_error(error_str.c_str());
}

static std::vector<float> generateVerticesFromData(const std::vector<vec3>& vertices_list, const std::vector<vec3>& normals_list, const std::vector<vec3>& uvs_list, const std::vector<unsigned int>& faces_indices, const std::vector<unsigned int>& normals_indices, const std::vector<unsigned int>& texture_indices){

    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec3> uvs;

    std::vector<float> vertices_buffer;

    try{
        if (faces_indices.size() > 0){
            for (unsigned int i = 0; i < faces_indices.size(); i++){
                if (faces_indices[i] >= vertices_list.size())
                    throwCustomIndexError("Face vertex [%d] index out of range", faces_indices[i] + 1);
                vec3 vertex = vertices_list[faces_indices[i]];
                vertices.push_back(vertex);

                if (normals_list.size() > 0){
                    if (i >= normals_indices.size()){
                        normals.push_back(vec3(1, 0, 0));
                    }
                    else{
                        if (normals_indices[i] >= normals_list.size())
                            throwCustomIndexError("Normal [%d] index out of range", normals_indices[i] + 1);
                        vec3 normal = normals_list[normals_indices[i]];
                        normals.push_back(normal);
                    }
                }
                else{
                    if (i % 3 == 2){
                        vec3 normal = generateNormal(vertices[i - 2], vertices[i - 1], vertices[i]);
                        normals.push_back(normal);
                        normals.push_back(normal);
                        normals.push_back(normal);
                    }
                }

                if (texture_indices.size() > 0){
                    if (i < texture_indices.size()){
                        if (texture_indices[i] < uvs_list.size()){
                            vec3 uv_coord = uvs_list[texture_indices[i]];
                            uvs.push_back(uv_coord);
                        }
                        else
                            throwCustomIndexError("Normal [%d] index out of range", texture_indices[i] + 1);
                    }
                    else{
                        uvs.push_back(vec3(0, 0, 0));
                        std::cout << "Pusheando 0\n";
                    }
                }
            }
        }
        else{
            for (unsigned int i = 0; i < vertices_list.size(); i++)
                vertices.push_back(vertices_list[i]);
            normals.push_back(vec3(1, 0, 0));
            uvs.push_back(vec3(0, 0, 0));
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::vector<float>(0);
    }

    if (uvs.size() == 0)
        uvs = generateUvs(vertices);
    for (unsigned int i = 0; i < vertices.size(); i++){
        vertices_buffer.push_back(vertices[i][0]);
        vertices_buffer.push_back(vertices[i][1]);
        vertices_buffer.push_back(vertices[i][2]);
        vertices_buffer.push_back(normals[i][0]);
        vertices_buffer.push_back(normals[i][1]);
        vertices_buffer.push_back(normals[i][2]);
        vertices_buffer.push_back(uvs[i][0]);
        vertices_buffer.push_back(uvs[i][1]);
    }


    return vertices_buffer;
}

static std::vector<float> centerVertices(const std::vector<float>& vertices){

    vec3 min_limit;
    vec3 max_limit;

    if (vertices.size() < 3)
        return (std::vector<float>(0));
    min_limit[0] = vertices[0];
    min_limit[1] = vertices[1];
    min_limit[2] = vertices[2];
    max_limit[0] = vertices[0];
    max_limit[1] = vertices[1];
    max_limit[2] = vertices[2];
    for (unsigned int i = 0; i < vertices.size(); i += 8){
        min_limit[0] = std::min(vertices[i], min_limit[0]);
        min_limit[1] = std::min(vertices[i + 1], min_limit[1]);
        min_limit[2] = std::min(vertices[i + 2], min_limit[2]);

        max_limit[0] = std::max(vertices[i], max_limit[0]);
        max_limit[1] = std::max(vertices[i + 1], max_limit[1]);
        max_limit[2] = std::max(vertices[i + 2], max_limit[2]);
    }

    vec3 middle;
    middle[0] = (min_limit[0] + max_limit[0]) / 2.0f;
    middle[1] = (min_limit[1] + max_limit[1]) / 2.0f;
    middle[2] = (min_limit[2] + max_limit[2]) / 2.0f;

    std::cout << "Middle: " << middle << std::endl;

    std::vector<float> centered_vertices = vertices;
    for (unsigned int i = 0; i < vertices.size(); i += 8){
        centered_vertices[i] = vertices[i] - middle[0];
        centered_vertices[i + 1] = vertices[i + 1] - middle[1];
        centered_vertices[i + 2] = vertices[i + 2] - middle[2];
    }
    return centered_vertices;
}


static vec3 generateNormal(vec3 v1, vec3 v2, vec3 v3){
    vec3 normal;

    normal = cross((v2 - v1).normalized(), (v3 - v1).normalized());

    return normal;
}

static std::vector<vec3> generateUvs(const std::vector<vec3>& vertices){
    vec3 min_limit;
    vec3 max_limit;
    enum PLANE { XY, XZ, YZ};

    min_limit[0] = vertices[0][0];
    min_limit[1] = vertices[0][1];
    min_limit[2] = vertices[0][2];
    max_limit[0] = vertices[0][0];
    max_limit[1] = vertices[0][1];
    max_limit[2] = vertices[0][2];
    for (unsigned int i = 0; i < vertices.size(); i++){
        min_limit[0] = std::min(vertices[i][0], min_limit[0]);
        min_limit[1] = std::min(vertices[i][1], min_limit[1]);
        min_limit[2] = std::min(vertices[i][2], min_limit[2]);

        max_limit[0] = std::max(vertices[i][0], max_limit[0]);
        max_limit[1] = std::max(vertices[i][1], max_limit[1]);
        max_limit[2] = std::max(vertices[i][2], max_limit[2]);
    }

    PLANE mapping_direction;
    float x_len = max_limit[0] - min_limit[0];
    float y_len = max_limit[1] - min_limit[1];
    float z_len = max_limit[2] - min_limit[2];

    float xy_plane_weight = x_len + y_len;
    float xz_plane_weight = x_len + z_len;
    float yz_plane_weight = y_len + z_len;

    if (xy_plane_weight >= xz_plane_weight && xy_plane_weight >= yz_plane_weight)
        mapping_direction = XY;
    else if (xz_plane_weight >= xy_plane_weight && xz_plane_weight > yz_plane_weight)
        mapping_direction = XZ;
    else
        mapping_direction = YZ;

    std::cout << "Min: " << min_limit << std::endl;
    std::cout << "Max: " << max_limit << std::endl;
    std::cout << "X_len: " << x_len << std::endl;
    std::cout << "Y_len: " << y_len << std::endl;
    std::cout << "Z_len: " << z_len << std::endl;
    std::cout << "Mapping direction: " << mapping_direction << std::endl;

    std::vector<vec3> uvs;
    vec3 uv;
    for (unsigned int i = 0; i < vertices.size(); i++){
        switch (mapping_direction)
        {
        case XY:
            uv[0] = (vertices[i][0] - min_limit[0])/x_len;
            uv[1] = (vertices[i][1] - min_limit[1])/y_len;
            break;
        case XZ:
            uv[0] = (vertices[i][0] - min_limit[0])/x_len;
            uv[1] = (vertices[i][2] - min_limit[2])/z_len;
            break;
        case YZ:
            uv[0] = (vertices[i][2] - min_limit[2])/z_len;
            uv[1] = (vertices[i][1] - min_limit[1])/y_len;
            break;
        
        default:
            uv[0] = (vertices[i][2] - min_limit[2])/z_len;
            uv[1] = (vertices[i][1] - min_limit[1])/y_len;
            break;
        }
        uvs.push_back(uv);
    }

    return uvs;
}
/* 
static std::vector<float> generateUvs(const std::vector<vec3> vertices_vector){
    std::vector<float> uvs;

    for (unsigned int i = 0; i < vertices_vector.size(); i++){
        uvs.push_back(0);
        uvs.push_back(0);
    }

    return uvs;
}

static std::vector<vec3> asignNormalsToVertices(const std::vector<vec3>& normals_list, const std::vector<unsigned int>& normals_indices, const unsigned int vertices_amount){
    std::vector<vec3> normals(vertices_amount);

    
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
} */