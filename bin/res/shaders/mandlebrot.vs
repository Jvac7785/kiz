#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 viewProjection;
uniform mat4 transform;

out mat4 uMVPMatrix;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 0, 1.0);
    uMVPMatrix = viewProjection * transform;
}
