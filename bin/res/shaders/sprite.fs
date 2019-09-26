#version 330 core

layout(location = 0) out vec4 color;

//in vec2 texCoord;

//uniform sampler2D tex;

void main()
{
    //color = texture2D(tex, texCoord);
    color = vec4(1.0, 0.0, 0.0, 1.0);
    /*if(color.w < 1.0)
    {
        discard;
    }*/
}