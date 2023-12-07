#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D shadowMap;

// Add a flag to identify luminous spheres
uniform int isLuminous;

// Define the position of the luminous sphere
uniform vec3 luminousSpherePos;

uniform int orbitDistance;

void main()
{
    // Fetch color from the color texture
    vec3 color = texture(colorTexture, TexCoord).xyz;

    // Fetch normal from the normal texture and transform it
    vec3 normalTex = texture(normalTexture, TexCoord).xyz * 2.0 - 1.0;
    vec3 normal = normalize(TBN * normalTex);

    // Set a constant brightness (adjust as needed)
    float brightness = 1.0;

    if (isLuminous == 1) {
        // Luminous sphere logic 
        // Calculate the vector from the fragment to the luminous sphere
        vec3 lightDir = normalize(luminousSpherePos - FragPos);

        // Calculate Lambertian diffuse reflection
        float diff = max(dot(normal, lightDir), 0.0);
        brightness += diff;

    } else {
        // Shadow mapping logic
        vec3 lightDir = normalize(luminousSpherePos - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        brightness = 1.0 - diff;

    }

        vec3 finalColor = color * brightness;

        FragColor = vec4(finalColor, 1.0);
  
}
