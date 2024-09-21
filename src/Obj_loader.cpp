#include <Obj_loader.hpp>

static std::vector<std::string> split(const std::string& s, char delimiter);
static std::vector<std::string> splitFaceInfo(const std::string& s);
static std::vector<float> generateVerticesFromData(const std::vector<vec3> &vertices_list, const std::vector<vec3> &normals_list, const std::vector<vec3> &uvs_list, const std::vector<unsigned int> &faces_indices, const std::vector<unsigned int> &normals_indices, const std::vector<unsigned int> &texture_indices);
static void processVertices(const std::vector<unsigned int> &faces_indices, unsigned int i, const std::vector<vec3> &vertices_list, std::vector<vec3> &vertices);
static void processTextureCoords(const std::vector<unsigned int> &texture_indices, unsigned int i, const std::vector<vec3> &uvs_list, std::vector<vec3> &uvs);
static void processNormals(const std::vector<vec3> &normals_list, unsigned int i, const std::vector<unsigned int> &normals_indices, std::vector<vec3> &normals, std::vector<vec3> &vertices);
static std::vector<float> centerVertices(const std::vector<float>& vertices);
static vec3 generateNormal(vec3& v1, vec3& v2, vec3& v3);
static std::vector<vec3> generateUvs(const std::vector<vec3>& vertices);

std::vector<float> loadObjFile(const std::string& path){

    if (path == ""){
        std::cout << "obj path is empty" << std::endl;
        return std::vector<float>(0);
    }

    std::ifstream file(path.c_str());
    if (!file.is_open()){
        std::cerr << "\e[96mOBJ ERROR opening file " << path << std::endl;
        return std::vector<float>(0);
    }

    std::vector<vec3> vertices_list;
    std::vector<vec3> normals_list;
    std::vector<vec3> uvs_list;
    std::vector<unsigned int> faces_indices;
    std::vector<unsigned int> normals_indices;
    std::vector<unsigned int> texture_indices;

    try{
        float x, y, z;
        std::string line;

        while(getline(file, line)){

            std::vector<std::string> parts = split(line, ' ');

            if (parts.size() == 0)
                continue;
            if (parts[0] == "v" && parts.size() == 4) {
                x = stof(parts[1]);
                y = stof(parts[2]);
                z = stof(parts[3]);

                vertices_list.push_back(vec3(x, y, z));
            }
            else if (parts[0] == "vn"){
                x = stof(parts[1]);
                y = stof(parts[2]);
                z = stof(parts[3]);

                normals_list.push_back(vec3(x, y, z)); 
            }
            else if (parts[0] == "vt"){
                x = stof(parts[1]);
                y = stof(parts[2]);

                uvs_list.push_back(vec3(x, y, 0));
            }
            else if (parts[0] == "f" && parts.size() >= 4){
               
                std::vector<std::string> first_face_parts = splitFaceInfo(parts[1]);

                for (unsigned int i = 2; i + 1 < parts.size(); i++){
                    std::vector<std::string> face_parts_1 = splitFaceInfo(parts[i]);
                    std::vector<std::string> face_parts_2 = splitFaceInfo(parts[i + 1]);

                    faces_indices.push_back(stoul(first_face_parts[0]) - 1);
                    faces_indices.push_back(stoul(face_parts_1[0]) - 1);
                    faces_indices.push_back(stoul(face_parts_2[0]) - 1);

                    if (!first_face_parts[1].empty() && !face_parts_1[1].empty() && !face_parts_2[1].empty()){
                        texture_indices.push_back(stoul(first_face_parts[1]) - 1);
                        texture_indices.push_back(stoul(face_parts_1[1]) - 1);
                        texture_indices.push_back(stoul(face_parts_2[1]) - 1);
                    }

                    if (!first_face_parts[2].empty() && !face_parts_1[2].empty() && !face_parts_2[2].empty()){
                        normals_indices.push_back(stoul(first_face_parts[2]) - 1);
                        normals_indices.push_back(stoul(face_parts_1[2]) - 1);
                        normals_indices.push_back(stoul(face_parts_2[2]) - 1);
                    }
                }      
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        file.close();
        return std::vector<float>(0);
    }
    
    file.close();
    
    std::vector<float> vertices = generateVerticesFromData(vertices_list, normals_list, uvs_list, faces_indices, normals_indices, texture_indices);
    std::vector<float> centered_vertices = centerVertices(vertices);
    return centered_vertices;
}

static std::vector<std::string> split(const std::string& s, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);

    while (getline(token_stream, token, delimiter)){
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
            if (n > 2)
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
        unsigned int data_amount = faces_indices.size();
        if (!data_amount)
            data_amount = vertices_list.size();

        for (unsigned int i = 0; i < data_amount; i++){

            if (faces_indices.size() > 0)
                processVertices(faces_indices, i, vertices_list, vertices);
            else
                vertices.push_back(vertices_list[i]);
            processNormals(normals_list, i, normals_indices, normals, vertices);
            processTextureCoords(texture_indices, i, uvs_list, uvs);
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

static void processVertices(const std::vector<unsigned int> &faces_indices, unsigned int i, const std::vector<vec3> &vertices_list, std::vector<vec3> &vertices)
{
    if (faces_indices[i] >= vertices_list.size())
        throwCustomIndexError("Face vertex [%d] index out of range", faces_indices[i] + 1);
    vertices.push_back(vertices_list[faces_indices[i]]);
}

static void processNormals(const std::vector<vec3> &normals_list, unsigned int i, const std::vector<unsigned int> &normals_indices, 
                            std::vector<vec3> &normals, std::vector<vec3> &vertices)
{
    if (normals_list.size() > 0)
    {
        if (i >= normals_indices.size())
            normals.push_back(vec3(1, 0, 0));
        else
        {
            if (normals_indices[i] >= normals_list.size())
                throwCustomIndexError("Normal [%d] index out of range", normals_indices[i] + 1);
            vec3 normal = normals_list[normals_indices[i]];
            normals.push_back(normal);
        }
    }
    else
    {
        if (i % 3 == 2)
        {
            vec3 normal = generateNormal(vertices[i - 2], vertices[i - 1], vertices[i]);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        }
    }
}

static vec3 generateNormal(vec3& v1, vec3& v2, vec3& v3)
{
    vec3 normal;
    normal = cross((v2 - v1).normalized(), (v3 - v1).normalized());
    return normal;
}

static void processTextureCoords(const std::vector<unsigned int> &texture_indices, unsigned int i, const std::vector<vec3> &uvs_list, std::vector<vec3> &uvs)
{
    if (texture_indices.size() > 0)
    {
        if (i < texture_indices.size())
        {
            if (texture_indices[i] < uvs_list.size())
            {
                vec3 uv_coord = uvs_list[texture_indices[i]];
                uvs.push_back(uv_coord);
            }
            else
                throwCustomIndexError("Normal [%d] index out of range", texture_indices[i] + 1);
        }
        else
        {
            uvs.push_back(vec3(0, 0, 0));
        }
    }
}

static std::vector<float> centerVertices(const std::vector<float>& vertices){

    vec3 min_limit, max_limit;

    if (vertices.size() < 3)
        return (std::vector<float>(0));
    min_limit[0] = max_limit[0] = vertices[0];
    min_limit[1] = max_limit[1] = vertices[1];
    min_limit[2] = max_limit[2] = vertices[2];

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

    std::vector<float> centered_vertices = vertices;
    for (unsigned int i = 0; i < vertices.size(); i += 8){
        centered_vertices[i] = vertices[i] - middle[0];
        centered_vertices[i + 1] = vertices[i + 1] - middle[1];
        centered_vertices[i + 2] = vertices[i + 2] - middle[2];
    }
    return centered_vertices;
}

static std::vector<vec3> generateUvs(const std::vector<vec3>& vertices){
    vec3 min_limit, max_limit;
    enum PLANE { XY, XZ, YZ};

    min_limit[0] = max_limit[0] = vertices[0][0];
    min_limit[1] = max_limit[1] = vertices[0][1];
    min_limit[2] = max_limit[2] = vertices[0][2];
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
            break;
        }
        uvs.push_back(uv);
    }

    return uvs;
}