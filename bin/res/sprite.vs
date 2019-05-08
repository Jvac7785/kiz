#version 130

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 tc;

uniform mat4 model;

void main()
{
   gl_Position = model * vec4(position, 0, 1.0);
   tc = texCoord;
}
