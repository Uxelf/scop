#include <Shader.hpp>

std::string getFileContent(const char* path);

unsigned int createProgramID(const char* vShaderCode, const char* fShaderCode);

Shader::Shader(const char* vertexPath, const char* fragmentPath){

    std::string vertexCode = getFileContent(vertexPath);
    std::string fragmentCode = getFileContent(fragmentPath);
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    ID = createProgramID(vShaderCode, fShaderCode);
    is_source = true;
}

Shader::Shader(const Shader& other){
    ID = other.ID;
    is_source = false;
}

Shader::~Shader(){
    if (is_source){
        glDeleteShader(ID);
    }
}

std::string getFileContent(const char* path){
    std::ifstream file;
    std::string code;

    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        file.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << file.rdbuf();		
        // close file handlers
        file.close();
        // convert stream into string
        code = shaderStream.str();
    }
    catch(const std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        std::cout << e.what() << std::endl;
    }
    return code;
}

unsigned int createProgramID(const char* vShaderCode, const char* fShaderCode){
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ID;
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 
void Shader::setMat4(const std::string& name, const mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.value_ptr()); 
}
void Shader::setVec3(const std::string& name, const float x, const float y, const float z)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}
void Shader::setVec3(const std::string& name, const vec3& vec)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2]); 
}