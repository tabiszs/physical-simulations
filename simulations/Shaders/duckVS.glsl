#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 modelMtx;
uniform mat4 projViewMtx;

void main()
{
	gl_Position = projViewMtx * modelMtx * vec4(aPos, 1.0);
	Color = vec3(aColor.x, aColor.y, aColor.z);
}