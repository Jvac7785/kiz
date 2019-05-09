#version 130

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 tc;

uniform mat4 model;
uniform mat4 pr;

void main()
{
   gl_Position = pr * model * vec4(position, 0, 1.0);
   tc = texCoord;
}
