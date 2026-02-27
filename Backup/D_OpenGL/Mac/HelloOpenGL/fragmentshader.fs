#version 410 core
out vec4 fragmentColor;

in vec3 vertex_color;
in vec2 uv;

uniform sampler2D MaiZhiMeng;

void main()
{
    fragmentColor = texture(MaiZhiMeng,uv);
}