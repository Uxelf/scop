#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform bool useTexture;

uniform vec3 objectColor;

layout (std140) uniform Lights
{
    vec3 ambientLightColor;
    vec3 lightColor;
    vec3 lightPos;
};


void main()
{
    vec3 ambientLightColor = vec3(0.1f);
    vec3 lightColor = vec3(1.0f);
    vec3 lightPos = vec3(1.0f, 1.0f, 1.0f);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result;
    if (useTexture)
        result = (ambientLightColor + diffuse) * (texture(objectTexture, TexCoord).rgb * objectColor);
    else
        result = (ambientLightColor + diffuse) * objectColor;
    FragColor = vec4(result, 1.0); 
}