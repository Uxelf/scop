#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D objectTexture;
uniform vec3 objectColor;
uniform bool useTexture;

void main()
{
    if (useTexture)
        FragColor = (texture(objectTexture, TexCoord).rgb * objectColor, 1.0f);
    else
        FragColor = objectColor;
}