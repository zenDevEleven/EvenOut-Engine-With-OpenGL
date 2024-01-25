#version 450 core

out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Texture[32];


const vec3 colorKey = vec3(1.0, 0.0, 1.0);
const float threshold = 0.1;

void main()
{
    vec4 texColor = texture(u_Texture[int(v_TexIndex)], v_TexCoord);


    if (distance(texColor.rgb, colorKey) < threshold)
        discard;

    color = texColor;
}