#include <Material.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static bool isPowerOfTwo(int n){
    return (n > 0) && (n & (n - 1)) == 0;
}

Material::Material(const Shader& shader, const vec3& color, const std::string& texturePath):
    _shader(shader), _color(color) {
    if (texturePath == ""){
        _use_texture = false;
        return;
    }

    stbi_set_flip_vertically_on_load(true);
    _texture.data = stbi_load(texturePath.c_str(), &_texture.width, &_texture.height, &_texture.nrChannels, 0);
    if (_texture.data){
        if (!isPowerOfTwo(_texture.width) || !isPowerOfTwo(_texture.height)){
            std::cout << "Texture width and/or height is not power of 2\n";
            stbi_image_free(_texture.data);
            return;
        }
        glGenTextures(1, &_texture.ID);
        glBindTexture(GL_TEXTURE_2D, _texture.ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texture.width, _texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, _texture.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
        stbi_image_free(_texture.data);
        _use_texture = true;
    }
    else{
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
}

Material::Material(const Material& other): 
    _shader(other._shader), _color(other._color), _texture(other._texture), _use_texture(other._use_texture) {
}

Material::~Material(){
}

void Material::setGenericShadersUniforms(){
    _shader.setVec3("objectColor", _color);
    _shader.setInt("useTexture", _use_texture);

    if (_use_texture)
        glBindTexture(GL_TEXTURE_2D, _texture.ID);
}