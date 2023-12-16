#version 400 core
layout(location = 0) out vec3 color;

uniform vec4 objectColor;

// cb lights

in vec3 worldPos;
in vec3 normal;
in vec3 viewVec;

const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);
const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
const float kd = 0.5, ks = 0.2f, m = 100.0f;

void main() {
   color = objectColor.xyz;
}