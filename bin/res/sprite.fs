#version 130

out vec4 out_Color;
varying vec2 tc;

uniform sampler2D tex;

void main()
{
    out_Color = texture2D(tex, tc);
    if(out_Color.w < 1.0)
    {
        discard;
    }
}