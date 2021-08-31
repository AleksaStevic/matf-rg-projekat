#version 330 core

in vec2 outTexCords;

uniform sampler2D t0;
uniform sampler2D t1;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(t0, outTexCords), texture(t1, outTexCords), 0.2);
}