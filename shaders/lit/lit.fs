#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

layout (std140) uniform Lights
{
    vec3 ambientLightColor;

    vec3 color;
    vec3 lightPosition;
    vec3 lightDiffuse;
	
    float lightConstant;
    float lightLinear;
    float lightQuadratic;
};

uniform sampler2D objectTexture;
uniform bool useTexture;

uniform float textureTransitionValue;
uniform vec3 objectColor;


void main()
{
    vec3 objectMixColor;
    if (useTexture){
        objectMixColor = mix(texture(objectTexture, TexCoord).rgb, objectColor, textureTransitionValue);
    }
    else {
        objectMixColor = objectColor;
    }
    
    vec3 ambient = ambientLightColor * objectMixColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * objectMixColor;

    // attenuation
    float distance = length(lightPosition - FragPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
    
    diffuse *= attenuation * 3;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0); 
}