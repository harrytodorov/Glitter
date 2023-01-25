#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

void main()
{
    FragColor = mix(texture(containerTexture, TexCoord), texture(faceTexture, TexCoord), 0.3);
}
