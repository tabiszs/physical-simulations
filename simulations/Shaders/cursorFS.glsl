#version 400 core
layout(location = 0) out vec3 color;

in vec3 Color;

void main() {
   color = Color.xyz;
}