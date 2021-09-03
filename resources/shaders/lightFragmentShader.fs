#version 330 core

//in vec2 outTexCords;

//uniform sampler2D t0;
//uniform sampler2D t1;

uniform vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0);
}