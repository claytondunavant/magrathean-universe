#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

void main()
{
    // Fetch color from the color texture
    vec3 color = texture(colorTexture, TexCoord).xyz;

    // Fetch normal from the normal texture and transform it
    vec3 normalTex = texture(normalTexture, TexCoord).xyz * 2.0 - 1.0;
    vec3 normal = normalize(TBN * normalTex);

    // Set a constant brightness (adjust as needed)
    float brightness = 1.0;

    // Combine color with constant brightness
    vec3 finalColor = color * brightness;

    FragColor = vec4(finalColor, 1.0);
    
} 