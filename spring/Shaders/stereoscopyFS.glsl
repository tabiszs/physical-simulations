#version 400 core
layout(location = 0) out vec3 color;

in vec2 TexCoord;

uniform sampler2D leftEye;
uniform sampler2D rightEye;

void main() {
	vec3 l = texture(leftEye, TexCoord).xyz;
	vec3 r = texture(rightEye, TexCoord).xyz;
	float left = (l.x + l.y + l.z ) /3.0f;
	float right = (r.x + r.y + r.z ) /3.0f;
	color = vec3(left, right, right);
}