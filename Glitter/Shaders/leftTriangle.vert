#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float horizontalOffset;

out vec3 ourColor;
out vec3 ourPosition;
void main()
{
   ourPosition = vec3(aPos.x + horizontalOffset, (-1.0) * aPos.y, aPos.z);
   gl_Position = vec4(ourPosition, 1.0);
   ourColor = aColor;
}
