#version 330 core

layout(location = 0) out vec4 color;

in vec2 tc;

uniform sampler2D tex;

void main()
{
    color = texture2D(tex, tc);
    if(color.w < 1)
    {
        discard;
    }
}
