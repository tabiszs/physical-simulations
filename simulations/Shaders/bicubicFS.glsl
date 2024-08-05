#version 400 core
layout(location = 0) out vec3 color;

uniform vec4 objectColor;

void main() {
   color = objectColor.xyz;
}