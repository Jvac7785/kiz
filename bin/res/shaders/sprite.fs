#version 330

layout(location = 0) out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
    color = texture2D(tex, texCoord);
    if(color.w < 1.0)
    {
        discard;
    }
}