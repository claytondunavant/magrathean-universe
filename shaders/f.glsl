#version 330 core

smooth in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
     FragColor = texture(textureSampler, TexCoord);
     //FragColor = vec4(TexCoord, 0.0, 1.0);
} 