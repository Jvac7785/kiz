#version 330 core

layout(location = 0) in vec2 position;
//layout(location = 1) in vec2 texCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

void main()
{
   gl_Position = viewProjection * transform * vec4(position, 0, 1.0);
   //tc = texCoord;
}