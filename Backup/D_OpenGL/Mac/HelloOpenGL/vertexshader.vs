#version 410 core

layout (location = 0) in vec3 _vertex_position;
layout (location = 1) in vec3 _vertex_color;
layout (location = 2) in vec2 _uv;

uniform mat4 transform;

out vec3 vertex_color;
out vec2 uv;

void main(){
    gl_Position = transform * vec4(_vertex_position,1.0f);
    vertex_color = _vertex_color;
    uv = _uv;
}