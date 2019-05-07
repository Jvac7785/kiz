#version 130

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 tc;

uniform mat4 scale;
uniform mat4 transform;
uniform mat4 pr;

void main()
{
    gl_Position.xy = vec4(position, 0.0, 1.0).xy;
    //gl_Position.xy = (pr * transform * scale * vec4(position, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    tc = texCoord;
}
